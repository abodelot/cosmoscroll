#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#include "PlayerShip.hpp"
#include "EntityManager.hpp"
#include "core/Game.hpp"
#include "core/ParticleSystem.hpp"
#include "core/SoundSystem.hpp"
#include "core/PlayerSave.hpp"
#include "items/ItemManager.hpp"
#include "utils/I18n.hpp"
#include "utils/MediaManager.hpp"
#include "utils/Math.hpp"

#define WEAPON1_OFFSET              52, 22
#define WEAPON2_OFFSET              50, 24

#define BONUS_SPEED_FACTOR 1.5
#define COOLER_DEFAULT              0
#define COOLER_MAX                  3

#define MISSILES_DEFAULT            0
#define MISSILES_MAX                3

#define HEAT_RECOVERY_RATE          13

#define SHIELD_RECOVERY_DELAY       6 // time to wait before earning another shield point

#define TIMED_BONUS_DURATION        10 // seconds


PlayerShip::PlayerShip(const sf::Vector2f& position, const char* animation) :
	Entity(position, -1), // hack, init HP later (we must load items first)
	Animated(EntityManager::GetInstance().GetAnimation(animation), *this),
	input_(Input::GetInstance()),
	panel_(ControlPanel::GetInstance())
{
	SetTeam(Entity::GOOD);
	SetSubRect(GetAnimation().GetFrame(0)); // WTF

	// init weapons
	weapon1_.Init("hellfire");
	weapon1_.SetOwner(this);
	weapon1_.SetOffset(WEAPON1_OFFSET);

	weapon2_.Init("laser-blue");
	weapon2_.SetOwner(this);
	weapon2_.SetOffset(WEAPON2_OFFSET);

	missile_launcher_.Init("missile");
	missile_launcher_.SetOwner(this);
	missile_launcher_.SetOffset(WEAPON2_OFFSET);

	coolers_ = COOLER_DEFAULT;
	missiles_ = MISSILES_DEFAULT;
	overheated_ = false;
	shield_timer_ = 0;
	shield_ = 0;
	heat_ = -1;

	speed_x_ = speed_y_ = 0.f;

	// init timed bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		bonus_[i] = 0.f;
	}

	// init control panel
	panel_.SetCoolers(coolers_);
	panel_.SetMissiles(missiles_);
	panel_.ActiveSpeedBonus(0);
	panel_.ActiveAttackBonus(0, Bonus::DOUBLE_SHOT);

	// init Konami code
	konami_code_[0] = Input::MOVE_UP;
	konami_code_[1] = Input::MOVE_UP;
	konami_code_[2] = Input::MOVE_DOWN;
	konami_code_[3] = Input::MOVE_DOWN;
	konami_code_[4] = Input::MOVE_LEFT;
	konami_code_[5] = Input::MOVE_RIGHT;
	konami_code_[6] = Input::MOVE_LEFT;
	konami_code_[7] = Input::MOVE_RIGHT;
	konami_code_[8] = Input::USE_WEAPON_2;
	konami_code_[9] = Input::USE_WEAPON_1;
	current_konami_event_ = 0;
	konami_code_activated_ = false;

	Initialize();
}


PlayerShip::~PlayerShip()
{
	ParticleSystem::GetInstance().RemoveShield(this);
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void PlayerShip::Initialize()
{
	const PlayerSave& save = Game::GetInstance().GetPlayerSave();
	const ItemManager& items = ItemManager::GetInstance();

	// points
	SetPoints(0);
	panel_.SetPoints(0);

	// shield
	shield_max_ = items.GetGenericItemData(ItemData::SHIELD, save.LevelOf(ItemData::SHIELD))->GetValue();
	panel_.SetMaxShield(shield_max_);
	panel_.SetShield(shield_);

	// ship armor
	int hp = items.GetGenericItemData(ItemData::ARMOR, save.LevelOf(ItemData::ARMOR))->GetValue();
	if (GetHP() == -1)
	{
		puts("respawn at full HP"); // DEBUG
		SetHP(hp);
	}
	hp_max_ = hp;
	panel_.SetMaxShipHP(hp);
	panel_.SetShipHP(GetHP());

	// engine
	speed_max_ = items.GetGenericItemData(ItemData::ENGINE, save.LevelOf(ItemData::ENGINE))->GetValue();

	// heat sink
	heat_max_ = items.GetGenericItemData(ItemData::HEATSINK, save.LevelOf(ItemData::HEATSINK))->GetValue();
	if (heat_ == -1)
	{
		puts("respawn at 0 heat");
		heat_ = 0.f;
	}
	panel_.SetMaxHeat(heat_max_);
	panel_.SetHeat(heat_);

	// weapon1
	int weapon1_lvl = save.LevelOf(ItemData::LASER1);
	const WeaponData* weapon1_data = items.GetWeaponData("hellfire", weapon1_lvl);
	weapon1_data->InitWeapon(&weapon1_);

	// weapon2
	int weapon2_lvl = save.LevelOf(ItemData::LASER2);
	const WeaponData* weapon2_data = items.GetWeaponData("laser-blue", weapon2_lvl);
	weapon2_data->InitWeapon(&weapon2_);
}


void PlayerShip::UpdateScoreCounter(int diff)
{
	int points = GetPoints() + diff;
	panel_.SetPoints(points);
	SetPoints(points);
}


PlayerShip* PlayerShip::Clone() const
{
	return new PlayerShip(*this);
}


void PlayerShip::HandleAction(Input::Action action)
{
	switch (action)
	{
		case Input::USE_COOLER:
			if (coolers_ > 0)
			{
				--coolers_;
				panel_.SetCoolers(coolers_);
				heat_ = 0.f;
				overheated_ = false;
				panel_.SetOverheat(false);
			}
			else
			{
				SoundSystem::GetInstance().PlaySound("disabled");
			}
			break;
		case Input::USE_MISSILE:
			if (missiles_ > 0 && missile_launcher_.IsReady())
			{
				--missiles_;
				panel_.SetMissiles(missiles_);
				missile_launcher_.Shoot(0);
			}
			else
			{
				SoundSystem::GetInstance().PlaySound("disabled");
			}
			break;
		case Input::COUNT: // filter non-events
			return;
		default:
			break;
	}

	// konami code
	if (action == konami_code_[current_konami_event_])
	{
		++current_konami_event_;
		if (current_konami_event_ == KONAMI_CODE_LENGTH)
		{
			current_konami_event_ = 0;
			KonamiCodeOn();
		}
	}
	else
	{
		current_konami_event_ = 0;
	}
}


void PlayerShip::Update(float frametime)
{
	static const EntityManager& manager = EntityManager::GetInstance();

	// animation
	Animated::Update(frametime, *this);

	// tirs
	if (!overheated_)
	{
		float h = 0.0f;
		if (input_.HasInput(Input::USE_WEAPON_1))
		{
			h += weapon1_.Shoot(0);
		}
		if (input_.HasInput(Input::USE_WEAPON_2))
		{
			h += weapon2_.Shoot(0);
		}

		heat_ += h;
		if (heat_ >= heat_max_)
		{
			overheated_ = true;
			panel_.SetOverheat(true);
			ParticleSystem::GetInstance().AddMessage(GetPosition(), _t("panel.overheat"));
		}
	}
	else if (input_.HasInput(Input::USE_WEAPON_1) || input_.HasInput(Input::USE_WEAPON_2))
	{
		SoundSystem::GetInstance().PlaySound("disabled");
	}

	// déplacement
	ComputeMove(frametime);
	sf::Vector2f pos = GetPosition();
	pos.y = pos.y + speed_y_ * frametime;
	pos.x = pos.x + speed_x_ * frametime;


	const int X_BOUND = manager.GetWidth() - GetSize().x;
	const int Y_BOUND = manager.GetHeight() - GetSize().y;

	if (pos.y < 0)
	{
		pos.y = 0;
	}
	else if (pos.y > Y_BOUND)
	{
		pos.y = Y_BOUND;
	}
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	else if (pos.x > X_BOUND)
	{
		pos.x = X_BOUND;
	}
	SetPosition(pos);

	// regénération bouclier
	if (shield_ < shield_max_)
	{
		shield_timer_ += frametime;
		if (shield_timer_ >= SHIELD_RECOVERY_DELAY)
		{
			IncreaseShield();
			shield_timer_ = 0.f;
		}
	}

	// refroidissement
	if (heat_ > 0.f)
	{
		heat_ -= HEAT_RECOVERY_RATE * frametime;
		if (heat_ <= 0.f)
		{
			heat_ = 0.f;
			if (overheated_ )
			{
				overheated_ = false;
				panel_.SetOverheat(false);
			}
		}
	}
	panel_.SetHeat(static_cast<int>(heat_));

	// timer bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		if (bonus_[i] > 0)
		{
			bonus_[i] -= frametime;
			if (bonus_[i] <= 0)
			{
				bonus_[i] = 0;
				DisableTimedBonus((TimedBonus) i);
			}
		}
	}
	// update armes
	weapon1_.Update(frametime);
	weapon2_.Update(frametime);
	missile_launcher_.Update(frametime);
}


void PlayerShip::TakeDamage(int damage)
{
	static ParticleSystem& p = ParticleSystem::GetInstance();
	if (shield_ > 0)
	{
		shield_ -= damage;
		p.RemoveShield(this);
		if (shield_ > 0)
		{
			p.AddShield(shield_, this);
		}
		else
		{
			SoundSystem::GetInstance().PlaySound("warp");
			shield_ = 0;
		}
		panel_.SetShield(shield_);
	}
	else
	{
		Entity::TakeDamage(damage);
		panel_.SetShipHP(GetHP());
		if (IsDead())
		{
			p.AddExplosion(GetPosition());
			EntityManager::GetInstance().TerminateGame();
		}
	}
}


void PlayerShip::OnCollide(Entity& entity)
{
	Bonus* bonus = dynamic_cast<Bonus*>(&entity);
	if (bonus != NULL)
	{
		HandleBonus(bonus->GetType());
		ParticleSystem::GetInstance().AddMessage(bonus->GetPosition(), bonus->GetDescription());
		SoundSystem::GetInstance().PlaySound("bonus");
		entity.Kill();
	}
	else
	{
		Entity::OnCollide(entity);
	}
}


bool PlayerShip::PixelPerfectCollide() const
{
	return true;
}



void PlayerShip::ComputeMove(float)
{
	speed_x_ = speed_y_ = 0;
	if (input_.HasInput(Input::MOVE_UP))
	{
		speed_y_ = -speed_max_;
	}
	else if (input_.HasInput(Input::MOVE_DOWN))
	{
		speed_y_ = speed_max_;
	}
	if (input_.HasInput(Input::MOVE_LEFT))
	{
		speed_x_ = -speed_max_;
	}
	else if (input_.HasInput(Input::MOVE_RIGHT))
	{
		speed_x_ = speed_max_;
	}
}


void PlayerShip::HandleBonus(Bonus::Type bonus_t)
{
	switch (bonus_t)
	{
		// timed bonus
		case Bonus::DOUBLE_SHOT:
			if (bonus_[T_DOUBLESHOT] == 0)
			{
				weapon1_.SetMultiply(2);
				weapon2_.SetMultiply(2);
			}
			bonus_[T_TRISHOT] = 0;
			bonus_[T_DOUBLESHOT] += TIMED_BONUS_DURATION;
			panel_.ActiveAttackBonus(bonus_[T_DOUBLESHOT], bonus_t);
			break;
		case Bonus::TRIPLE_SHOT:
			if (bonus_[T_TRISHOT] == 0)
			{
				weapon1_.SetMultiply(3);
				weapon2_.SetMultiply(3);
			}
			bonus_[T_DOUBLESHOT] = 0;
			bonus_[T_TRISHOT] += TIMED_BONUS_DURATION;
			panel_.ActiveAttackBonus(bonus_[T_TRISHOT], bonus_t);
			break;
		case Bonus::SPEED:
			if (bonus_[T_SPEED] == 0)
			{
				speed_max_ *= BONUS_SPEED_FACTOR;
				ParticleSystem::GetInstance().AddSmoke(96, this);
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			panel_.ActiveSpeedBonus(bonus_[T_SPEED]);
			break;
		// immediate bonus
		case Bonus::SUPER_BANANA:
			ParticleSystem::GetInstance().AddFiery(
				GetPosition().x + GetSize().x / 2,
				GetPosition().y + GetSize().y / 2);
			// max hp
			SetHP(hp_max_);
			panel_.SetShipHP(hp_max_);
			// max shield
			IncreaseShield(shield_max_ - shield_);
			// +1 missile, +1 cooler
			HandleBonus(Bonus::MISSILE);
			HandleBonus(Bonus::COOLER);
			break;
		case Bonus::HEALTH:
			if (GetHP() < hp_max_)
			{
				panel_.SetShipHP(UpdateHP(1));
			}
			break;
		case Bonus::SHIELD:
			if (shield_ < shield_max_)
			{
				IncreaseShield();
			}
			break;
		case Bonus::COOLER:
			if (coolers_ < COOLER_MAX)
			{
				++coolers_;
				panel_.SetCoolers(coolers_);
			}
			break;
		case Bonus::MISSILE:
			if (missiles_ < MISSILES_MAX)
			{
				++missiles_;
				panel_.SetMissiles(missiles_);
			}
			break;
		default:
			break;
	}
}


void PlayerShip::DisableTimedBonus(TimedBonus tbonus)
{
	switch (tbonus)
	{
		case T_DOUBLESHOT:
		case T_TRISHOT:
			weapon1_.SetMultiply(1);
			weapon2_.SetMultiply(1);
			break;
		case T_SPEED:
			speed_max_ /= BONUS_SPEED_FACTOR;
			ParticleSystem::GetInstance().ClearSmoke(this);
			break;
		default:
			break;
	}
	bonus_[tbonus] = 0;
}


void PlayerShip::IncreaseShield(int count)
{
	shield_ += count;
	// update particles
	ParticleSystem& p = ParticleSystem::GetInstance();
	p.RemoveShield(this);
	p.AddShield(shield_, this);
	// update panel count
	panel_.SetShield(shield_);
}


void PlayerShip::KonamiCodeOn()
{
	konami_code_activated_ = true;
	HandleBonus(Bonus::SUPER_BANANA);
	HandleBonus(Bonus::SPEED);
	coolers_ = 42;
	panel_.SetCoolers(42);
	missiles_ = 42;
	panel_.SetMissiles(42);
	hp_max_ = 9000;
	SetHP(hp_max_);
	panel_.SetMaxShipHP(hp_max_);
	panel_.SetShipHP(hp_max_);

	weapon1_.SetMultiply(3);
	weapon2_.SetMultiply(2);
	missile_launcher_.SetMultiply(3);

	ParticleSystem::GetInstance().AddMessage(GetPosition(), L"For great justice!");
}



