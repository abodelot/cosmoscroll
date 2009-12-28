#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#include "PlayerShip.hpp"
#include "EntityManager.hpp"
#include "../core/ParticleSystem.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"
#include "../utils/DIE.hpp"

#define WEAPON1_OFFSET              52, 22
#define WEAPON2_OFFSET              50, 24

#define MAX_SPEED                   200.f
#define BONUS_MAX_SPEED             350.f
#define ACCELERATION_DELAY_ON_DRUGS 0.4f // time to reach MAX_SPEED from 0 (or 0 from MAX_SPEED)

#define COOLER_DEFAULT              0
#define COOLER_MAX                  3

#define MISSILES_DEFAULT            0
#define MISSILES_MAX                3

#define HEAT_MAX                    100
#define HEAT_RECOVERY_RATE          13

#define HP_DEFAULT                  3
#define HP_MAX                      5

#define SHIELD_DEFAULT              0 // shield points at start
#define SHIELD_MAX                  6 // max shield points
#define SHIELD_RECOVERY_RATE        0.2 // shield point per second

#define TIMED_BONUS_DURATION        10 // seconds


PlayerShip::PlayerShip(const sf::Vector2f& position, const char* animation) :
	Entity(position, HP_DEFAULT),
	Animated(EntityManager::GetInstance().GetAnimation(animation), *this),
	input_(Input::GetInstance()),
	panel_(ControlPanel::GetInstance())
{
	SetTeam(Entity::GOOD);
	SetSubRect(GetAnimation().GetFrame(0)); // WTF
	// init weapons
	EntityManager& mgr = EntityManager::GetInstance();
	mgr.InitWeapon(2, &weapon1_);
	weapon1_.SetOwner(this);
	weapon1_.SetOffset(WEAPON1_OFFSET);

	mgr.InitWeapon(1, &weapon2_);
	weapon2_.SetOwner(this);
	weapon2_.SetOffset(WEAPON2_OFFSET);

	mgr.InitWeapon(5, &missile_launcher_);
	missile_launcher_.SetOwner(this);
	missile_launcher_.SetOffset(WEAPON2_OFFSET);

	shield_ = SHIELD_DEFAULT;
	coolers_ = COOLER_DEFAULT;
	missiles_ = MISSILES_DEFAULT;
	overheated_ = false;
	shield_timer_ = 0;
	heat_ = 0.0f;

	max_speed_ = MAX_SPEED;
	speed_x_ = speed_y_ = 0.f;
	compute_move_ = &PlayerShip::ComputeMove;

	ParticleSystem::GetInstance().AddShield(SHIELD_DEFAULT, this);

	// init timed bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		bonus_[i] = 0.f;
	}

	// init control panel
	panel_.SetMaxShipHP(HP_MAX);
	panel_.SetMaxShield(SHIELD_MAX);
	panel_.SetMaxHeat(HEAT_MAX);
	panel_.SetShipHP(hp_);
	panel_.SetShield(shield_);
	panel_.SetHeat((int) heat_);
	panel_.SetCoolers(coolers_);
	panel_.SetMissiles(missiles_);
	panel_.SetOverheatText("");

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
}


PlayerShip::~PlayerShip()
{
	ParticleSystem::GetInstance().RemoveShield(this);
	ParticleSystem::GetInstance().ClearSmoke(this);
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
				panel_.SetOverheatText("");
			}
			break;
		case Input::USE_MISSILE:
			if (missiles_ > 0 && missile_launcher_.IsReady())
			{
				--missiles_;
				panel_.SetMissiles(missiles_);
				missile_launcher_.Shoot(GetPosition(), 0);
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
	// animation
	Animated::Update(frametime, *this);

	// tirs
	if (!overheated_)
	{
		float h = 0.0f;
		if (input_.HasInput(Input::USE_WEAPON_1))
		{
			h += weapon1_.Shoot(GetPosition(), 0);
		}
		if (input_.HasInput(Input::USE_WEAPON_2))
		{
			h += weapon2_.Shoot(GetPosition(), 0);
		}

		heat_ += h;
		if (heat_ >= HEAT_MAX)
		{
			overheated_ = true;
			panel_.SetOverheatText("Surchauffe !");
		}
	}
	// déplacement
	(this->*compute_move_)(frametime);
	sf::Vector2f pos = GetPosition();
	pos.y = pos.y + speed_y_ * frametime;
	pos.x = pos.x + speed_x_ * frametime;

	static const EntityManager& manager = EntityManager::GetInstance();
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
	if (shield_ < SHIELD_MAX)
	{
		shield_timer_ -= frametime;
		if (shield_timer_ <= 0.f)
		{
			IncreaseShield();
			shield_timer_ = 1 / SHIELD_RECOVERY_RATE;
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
				panel_.SetOverheatText("");
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
		panel_.SetShipHP(hp_ > 0 ? hp_ : 0);
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
		speed_y_ = -max_speed_;
	}
	else if (input_.HasInput(Input::MOVE_DOWN))
	{
		speed_y_ = max_speed_;
	}
	if (input_.HasInput(Input::MOVE_LEFT))
	{
		speed_x_ = -max_speed_;
	}
	else if (input_.HasInput(Input::MOVE_RIGHT))
	{
		speed_x_ = max_speed_;
	}
}


void PlayerShip::ComputeMoveOnDrugs(float frametime)
{
	float speed_diff = frametime * max_speed_ / ACCELERATION_DELAY_ON_DRUGS;
	ComputeAxisSpeed(speed_x_, Input::MOVE_LEFT, Input::MOVE_RIGHT, speed_diff);
	ComputeAxisSpeed(speed_y_, Input::MOVE_UP, Input::MOVE_DOWN, speed_diff);
}


void PlayerShip::ComputeAxisSpeed(float& speed, Input::Action lower,
	Input::Action upper, float diff)
{
	if (input_.HasInput(lower) && -speed < max_speed_)
	{
		speed -= diff;
	}
	else if (speed < 0)
	{
		speed += diff;
		if (speed > 0.1)
		{
			speed = 0;
		}
	}

	if (input_.HasInput(upper) && speed < max_speed_)
	{
		speed += diff;
	}
	else if (speed > 0)
	{
		speed -= diff;
		if (speed < 0.1)
		{
			speed = 0;
		}
	}
}


void PlayerShip::HandleBonus(Bonus::Type bonus_t)
{
	switch (bonus_t)
	{
		// timed bonus
		case Bonus::TRIPLE_SHOT:
			if (bonus_[T_TRISHOT] == 0)
			{
				weapon1_.SetTriple(true);
				weapon2_.SetTriple(true);
			}
			bonus_[T_TRISHOT] += TIMED_BONUS_DURATION;
			break;
		case Bonus::SPEED:
			if (bonus_[T_SPEED] == 0)
			{
				max_speed_ = BONUS_MAX_SPEED;
				ParticleSystem::GetInstance().AddSmoke(96, this);
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			break;
		case Bonus::STONED:
			if (bonus_[T_STONED] == 0)
			{
				compute_move_ = &PlayerShip::ComputeMoveOnDrugs;
			}
			bonus_[T_STONED] += TIMED_BONUS_DURATION;
			break;
		// immediate bonus
		case Bonus::SUPER_BANANA:
			ParticleSystem::GetInstance().AddFiery(
				GetPosition().x + GetSize().x / 2,
				GetPosition().y + GetSize().y / 2);
			// max hp
			hp_ = HP_MAX;
			panel_.SetShipHP(hp_);
			// max shield
			IncreaseShield(SHIELD_MAX - shield_);
			// no overheat
			heat_ = 0;
			overheated_ = false;
			panel_.SetOverheatText("");
			// +1 missile, +1 cooler
			HandleBonus(Bonus::MISSILE);
			HandleBonus(Bonus::COOLER);
			break;
		case Bonus::HEALTH:
			if (hp_ < HP_MAX)
			{
				++hp_;
				panel_.SetShipHP(hp_);
			}
			break;
		case Bonus::SHIELD:
			if (shield_ < SHIELD_MAX)
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
			DIE("can't handle unknown bonus");
			break;
	}
}


void PlayerShip::DisableTimedBonus(TimedBonus tbonus)
{
	switch (tbonus)
	{
		case T_TRISHOT:
			weapon1_.SetTriple(false);
			weapon2_.SetTriple(false);
			break;
		case T_SPEED:
			max_speed_ = MAX_SPEED;
			ParticleSystem::GetInstance().ClearSmoke(this);
			break;
		case T_STONED:
			compute_move_ = &PlayerShip::ComputeMove;
			break;
		default:
			DIE("can't disable unknown bonus");
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
	HandleBonus(Bonus::SUPER_BANANA);
	HandleBonus(Bonus::SPEED);
	coolers_ = 42;
	panel_.SetCoolers(42);
	missiles_ = 42;
	panel_.SetMissiles(42);
	weapon1_.SetTriple(true);
	weapon2_.SetTriple(true);
	missile_launcher_.SetTriple(true);

	ParticleSystem::GetInstance().AddMessage(GetPosition(), L"Have you mooed today?");
}



