#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#include "Player.hpp"
#include "EntityManager.hpp"
#include "core/PlayerSave.hpp"
#include "core/ParticleSystem.hpp"
#include "core/MessageSystem.hpp"
#include "core/SoundSystem.hpp"
#include "core/PlayerSave.hpp"
#include "core/Resources.hpp"
#include "items/ItemManager.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"

#define WEAPON_POSITION             sf::Vector2f(54, 24)

#define BONUS_SPEED_FACTOR 1.5
#define COOLER_DEFAULT              0
#define COOLER_MAX                  3

#define MISSILES_DEFAULT            0
#define MISSILES_MAX                3

// time to wait from overheat to heat 0 (seconds)
#define COOLING_DELAY              10

// time to wait before earning another shield point (seconds)
#define SHIELD_RECOVERY_DELAY       6

#define TIMED_BONUS_DURATION        10


Player::Player():
	input_(Input::GetInstance()),
	panel_(ControlPanel::GetInstance())
{
	setTeam(Entity::GOOD);
	setHP(-1);
	m_animator.setAnimation(*this, EntityManager::getInstance().getAnimation("player"));

	// init weapons
	m_weapon.init("laser-red");
	m_weapon.setOwner(this);
	m_weapon.setPosition(WEAPON_POSITION);

	m_missile_launcher.init("missile");
	m_missile_launcher.setOwner(this);
	m_missile_launcher.setPosition(WEAPON_POSITION);

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
	panel_.SetOverheat(false);
	panel_.ActiveSpeedPowerUp(0);
	panel_.ActiveAttackPowerUp(0, PowerUp::DOUBLE_SHOT);

	// init Konami code sequence
	m_konami_code[0] = Input::MOVE_UP;
	m_konami_code[1] = Input::MOVE_UP;
	m_konami_code[2] = Input::MOVE_DOWN;
	m_konami_code[3] = Input::MOVE_DOWN;
	m_konami_code[4] = Input::MOVE_LEFT;
	m_konami_code[5] = Input::MOVE_RIGHT;
	m_konami_code[6] = Input::MOVE_LEFT;
	m_konami_code[7] = Input::MOVE_RIGHT;
	m_konami_code[8] = Input::USE_MISSILE;
	m_konami_code[9] = Input::USE_LASER;
	m_current_konami_index = 0;
	m_konami_code_activated = false;
}


Player::~Player()
{
	ParticleSystem::GetInstance().RemoveShield(this);
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void Player::onInit()
{
	const ItemManager& items = ItemManager::GetInstance();

	// score
	m_score = 0;
	panel_.setScore(0);

	// shield
	shield_max_ = items.GetGenericItemData(ItemData::SHIELD, PlayerSave::getItemLevel(ItemData::SHIELD))->GetValue();
	panel_.SetMaxShield(shield_max_);
	setShield(shield_);

	// ship hull (hit points)
	int hp = items.GetGenericItemData(ItemData::HULL, PlayerSave::getItemLevel(ItemData::HULL))->GetValue();
	if (getHP() == -1) // keep previous HP value unless HP wasn't initialized yet
	{
		setHP(hp);
	}
	hp_max_ = hp;
	panel_.SetMaxShipHP(hp);
	panel_.SetShipHP(getHP());

	// engine
	speed_max_ = items.GetGenericItemData(ItemData::ENGINE, PlayerSave::getItemLevel(ItemData::ENGINE))->GetValue();

	// heat sink
	heat_max_ = items.GetGenericItemData(ItemData::HEATSINK, PlayerSave::getItemLevel(ItemData::HEATSINK))->GetValue();
	if (heat_ == -1) // keep previous heat value unless heat wasn't initialized yet
	{
		heat_ = 0.f;
	}
	panel_.SetMaxHeat(heat_max_);
	panel_.SetHeat(heat_);

	// weapon
	m_weapon.init("laser-red", PlayerSave::getItemLevel(ItemData::WEAPON));
}


int Player::getScore() const
{
	return m_score;
}


void Player::updateScore(int diff)
{
	m_score += diff;
	panel_.setScore(m_score);
}


void Player::AudibleHeatingCue()
{
	static float h_steps[] = {.50f, .65f, .80f, .90f};
	static int nb_steps = sizeof (h_steps) / sizeof (float);
	static int current_step = 0;

	float heat_pct_ = heat_ / heat_max_;
	if (current_step < nb_steps && heat_pct_ > h_steps[current_step])
	{
		SoundSystem::GetInstance().PlaySound("overheat.ogg");
		++current_step;
	}
	else if (current_step > 0 && heat_pct_ < h_steps[current_step -1])
	{
		--current_step;
	}
}


void Player::onEvent(const sf::Event& event)
{
	EntityManager& manager = EntityManager::getInstance();
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			if (input_.keyForAction(Input::MOVE_UP) == event.key.code)
				m_animator.setAnimation(*this, manager.getAnimation("player-up"));
			else if (input_.keyForAction(Input::MOVE_DOWN) == event.key.code)
				m_animator.setAnimation(*this, manager.getAnimation("player-down"));
			break;
		case sf::Event::KeyReleased:
			if (input_.keyForAction(Input::MOVE_UP) == event.key.code ||
				input_.keyForAction(Input::MOVE_DOWN) == event.key.code)
			{
				m_animator.setAnimation(*this, manager.getAnimation("player"));
			}
			break;
		default:
			break;
	}

	Input::Action action = input_.EventToAction(event);
	switch (action)
	{
		case Input::USE_COOLER:
			if (coolers_ > 0)
			{
				SoundSystem::GetInstance().PlaySound("cooler.ogg");
				ParticleSystem::GetInstance().SnowflakeSfx(getCenter(), 40);
				--coolers_;
				panel_.SetCoolers(coolers_);
				heat_ = 0.f;
				overheated_ = false;
				panel_.SetOverheat(false);
			}
			else
			{
				SoundSystem::GetInstance().PlaySound("disabled.ogg");
			}
			break;
		case Input::USE_MISSILE:
			if (missiles_ > 0 && m_missile_launcher.isReady())
			{
				--missiles_;
				panel_.SetMissiles(missiles_);
				m_missile_launcher.shoot(0);
			}
			else
			{
				SoundSystem::GetInstance().PlaySound("disabled.ogg");
			}
			break;
		case Input::USE_LASER:
			if (overheated_)
			{
				SoundSystem::GetInstance().PlaySound("disabled.ogg");
			}
			break;
		case Input::COUNT: // filter non-events // TODO: wat?
			return;
		default:
			break;
	}

	// Konami code
	if (action == m_konami_code[m_current_konami_index])
	{
		++m_current_konami_index;
		if (m_current_konami_index == KONAMI_CODE_LENGTH)
		{
			turnKonamiCodeOn();
			m_current_konami_index = 0; // Reset
		}
	}
	else
	{
		m_current_konami_index = 0;
	}

}


void Player::onUpdate(float frametime)
{
	static const EntityManager& manager = EntityManager::getInstance();

	// animation
	m_animator.updateSubRect(*this, frametime);

	// weapons
	if (!overheated_)
	{
		float h = 0.f;
		if (input_.HasInput(Input::USE_LASER))
		{
			h += m_weapon.shoot(0);
		}

		heat_ += h;
		if (heat_ >= heat_max_)
		{
			overheated_ = true;
			panel_.SetOverheat(true);
			MessageSystem::write(_t("panel.overheat"), getPosition());
		}
		if (h > 0)
			AudibleHeatingCue();
	}


	// moving
	Computemove(frametime);
	sf::Vector2f pos = getPosition();
	pos.y = pos.y + speed_y_ * frametime;
	pos.x = pos.x + speed_x_ * frametime;

	const int X_BOUND = manager.getWidth() - getWidth();
	const int Y_BOUND = manager.getHeight() - getHeight();

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
	setPosition(pos);

	// shield regeneration
	if (shield_ < shield_max_)
	{
		shield_timer_ += frametime;
		if (shield_timer_ >= SHIELD_RECOVERY_DELAY)
		{
			setShield(shield_ + 1);
			shield_timer_ = 0.f;
		}
	}

	// cooling
	if (heat_ > 0.f)
	{
		heat_ -= frametime * heat_max_ / COOLING_DELAY;
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
				DisableTimedPowerUp((TimedPowerUp) i);
			}
		}
	}
	// update weapons
	m_weapon.onUpdate(frametime);
	m_missile_launcher.onUpdate(frametime);

	updateDamageFlash(frametime);
}


void Player::takeDamage(int damage)
{
	if (damage == 0)
		return;

	static ParticleSystem& p = ParticleSystem::GetInstance();
	if (shield_ > 0)
	{
		shield_ -= damage;
		SoundSystem::GetInstance().PlaySound("shield-damage.ogg");
		p.RemoveShield(this);
		if (shield_ > 0)
		{
			p.AddShield(shield_, this);
		}
		else
		{
			shield_ = 0;
		}
		panel_.SetShield(shield_);
	}
	else
	{
		Damageable::takeDamage(damage);
		SoundSystem::GetInstance().PlaySound("ship-damage.ogg");
		panel_.SetShipHP(getHP());
	}
}


void Player::onCollision(PowerUp& powerup)
{
	switch (powerup.getType())
	{
		// timed bonus
		case PowerUp::DOUBLE_SHOT:
			if (bonus_[T_DOUBLESHOT] == 0)
			{
				m_weapon.setMultiply(2);
			}
			bonus_[T_TRISHOT] = 0;
			bonus_[T_DOUBLESHOT] += TIMED_BONUS_DURATION;
			panel_.ActiveAttackPowerUp(bonus_[T_DOUBLESHOT], powerup.getType());
			break;
		case PowerUp::TRIPLE_SHOT:
			if (bonus_[T_TRISHOT] == 0)
			{
				m_weapon.setMultiply(3);
			}
			bonus_[T_DOUBLESHOT] = 0;
			bonus_[T_TRISHOT] += TIMED_BONUS_DURATION;
			panel_.ActiveAttackPowerUp(bonus_[T_TRISHOT], powerup.getType());
			break;
		case PowerUp::SPEED:
			if (bonus_[T_SPEED] == 0)
			{
				speed_max_ *= BONUS_SPEED_FACTOR;
				ParticleSystem::GetInstance().AddSmoke(96, this);
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			panel_.ActiveSpeedPowerUp(bonus_[T_SPEED]);
			break;
		// immediate bonus
		case PowerUp::SUPER_BANANA:
			ParticleSystem::GetInstance().FierySfx(getCenter(), 50);
			setHP(hp_max_);
			panel_.SetShipHP(hp_max_);
			setShield(shield_max_);
			break;
		case PowerUp::HEALTH:
			if (getHP() < hp_max_)
			{
				panel_.SetShipHP(updateHP(1));
			}
			break;
		case PowerUp::SHIELD:
			if (shield_ < shield_max_)
			{
				setShield(shield_ + 1);
			}
			break;
		case PowerUp::COOLER:
			if (coolers_ < COOLER_MAX)
			{
				++coolers_;
				panel_.SetCoolers(coolers_);
			}
			break;
		case PowerUp::MISSILE:
			if (missiles_ < MISSILES_MAX)
			{
				++missiles_;
				panel_.SetMissiles(missiles_);
			}
			break;
		default:
			break;
	}

	powerup.kill();
	MessageSystem::write(powerup.getDescription(), powerup.getPosition());
	SoundSystem::GetInstance().PlaySound("power-up.ogg");
}


void Player::onDestroy()
{
	setColor(sf::Color::White); // clear red flash
	EntityManager& manager = EntityManager::getInstance();
	m_animator.setAnimation(*this, manager.getAnimation("player-destroyed"));

	manager.TerminateGame();
}


void Player::Computemove(float)
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


void Player::DisableTimedPowerUp(TimedPowerUp tbonus)
{
	switch (tbonus)
	{
		case T_DOUBLESHOT:
		case T_TRISHOT:
			m_weapon.setMultiply(1);
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


void Player::setShield(int count)
{
	shield_ = count;
	// update particles
	ParticleSystem& p = ParticleSystem::GetInstance();
	p.RemoveShield(this);
	p.AddShield(shield_, this);
	// update panel count
	panel_.SetShield(shield_);
}


void Player::turnKonamiCodeOn()
{
	m_konami_code_activated = true;

	// max hp
	setHP(hp_max_);
	panel_.SetShipHP(hp_max_);
	// max shield
	setShield(shield_max_);

	coolers_ = 42;
	panel_.SetCoolers(42);
	missiles_ = 42;
	panel_.SetMissiles(42);

	m_weapon.setMultiply(3);
	m_missile_launcher.setMultiply(3);

	MessageSystem::write("For great justice!", getPosition());
}
