#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#include "Player.hpp"
#include "EntityManager.hpp"
#include "core/UserSettings.hpp"
#include "core/MessageSystem.hpp"
#include "core/SoundSystem.hpp"
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
	m_speed(0.f),
	panel_(ControlPanel::getInstance()),
	// Preload animations
	m_animation_up(EntityManager::getInstance().getAnimation("player-up")),
	m_animation_down(EntityManager::getInstance().getAnimation("player-down")),
	m_animation_normal(EntityManager::getInstance().getAnimation("player")),
	m_score(0)
{
	setTeam(Entity::GOOD);
	setHP(-1);

	// Init sprite texture
	m_animator.setAnimation(*this, m_animation_normal);

	// Init weapons
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

	// Init timed bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		bonus_[i] = 0.f;
	}

	// Init control panel
	panel_.SetCoolers(coolers_);
	panel_.SetMissiles(missiles_);
	panel_.SetOverheat(false);
	panel_.ActiveSpeedPowerUp(0);
	panel_.ActiveAttackPowerUp(0, PowerUp::DOUBLE_SHOT);

	// Init Konami code sequence
	m_konami_code[0] = Action::UP;
	m_konami_code[1] = Action::UP;
	m_konami_code[2] = Action::DOWN;
	m_konami_code[3] = Action::DOWN;
	m_konami_code[4] = Action::LEFT;
	m_konami_code[5] = Action::RIGHT;
	m_konami_code[6] = Action::LEFT;
	m_konami_code[7] = Action::RIGHT;
	m_konami_code[8] = Action::USE_MISSILE;
	m_konami_code[9] = Action::USE_LASER;
	m_current_konami_index = 0;
	m_konami_code_activated = false;

	// Init particles emitters
	m_shield_emitter.setTextureRect(sf::IntRect(40, 0, 8, 8));
	m_shield_emitter.setLifetime(0);

	m_smoke_emitter.setTextureRect(sf::IntRect(0, 0, 16, 16));
	m_smoke_emitter.setLooping(true);
	m_smoke_emitter.setSpeed(50, 25);
	m_smoke_emitter.setLifetime(1.5);
	m_smoke_emitter.setAngle(-math::PI, 0.5);
	m_smoke_emitter.setScale(0.5, 1.5);

	m_snowflakes_emitter.setTextureRect(sf::IntRect(16, 0, 16, 16));
	m_snowflakes_emitter.setScale(0.2, 1.5);

	m_powerup_emitter.setTextureRect(sf::IntRect(32, 0, 8, 8));
}


Player::~Player()
{
	m_smoke_emitter.clearParticles();
	m_shield_emitter.clearParticles();
}


void Player::onInit()
{
	const ItemManager& items = ItemManager::GetInstance();

	// score
	m_score = 0;
	panel_.setScore(0);

	// shield
	shield_max_ = items.GetGenericItemData(ItemData::SHIELD, UserSettings::getItemLevel(ItemData::SHIELD))->GetValue();
	panel_.SetMaxShield(shield_max_);
	setShield(shield_);

	// ship hull (hit points)
	int hp = items.GetGenericItemData(ItemData::HULL, UserSettings::getItemLevel(ItemData::HULL))->GetValue();
	if (getHP() == -1) // keep previous HP value unless HP wasn't initialized yet
	{
		setHP(hp);
	}
	hp_max_ = hp;
	panel_.SetMaxShipHP(hp);
	panel_.SetShipHP(getHP());

	// engine
	m_speed = items.GetGenericItemData(ItemData::ENGINE, UserSettings::getItemLevel(ItemData::ENGINE))->GetValue();

	// heat sink
	heat_max_ = items.GetGenericItemData(ItemData::HEATSINK, UserSettings::getItemLevel(ItemData::HEATSINK))->GetValue();
	if (heat_ == -1) // keep previous heat value unless heat wasn't initialized yet
	{
		heat_ = 0.f;
	}
	panel_.SetMaxHeat(heat_max_);
	panel_.SetHeat(heat_);

	// weapon
	m_weapon.init("laser-red", UserSettings::getItemLevel(ItemData::WEAPON));
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


void Player::overheatAudioHint() const
{
	static float thresholds[] = {.50f, .60f, .70f, .80f, .90f};
	static int thresholds_count = sizeof (thresholds) / sizeof (float);
	static int current_index = 0;

	// Get heat value between 0 and 1
	float heat_percent = heat_ / heat_max_;
	if (current_index < thresholds_count && heat_percent > thresholds[current_index])
	{
		// New threshold reached
		SoundSystem::playSound("overheat.ogg", 0.4 + thresholds[current_index]);
		++current_index;
	}
	else if (current_index > 0 && heat_percent < thresholds[current_index -1])
	{
		--current_index;
	}
}


void Player::onActionDown(Action::ID action)
{
	switch (action)
	{
		case Action::UP:
			m_animator.setAnimation(*this, m_animation_up);
			break;
		case Action::DOWN:
			m_animator.setAnimation(*this, m_animation_down);
			break;
		case Action::USE_COOLER:
			if (coolers_ > 0)
			{
				// Play sound effect and launch particles
				SoundSystem::playSound("cooler.ogg");
				m_snowflakes_emitter.setPosition(getCenter());
				m_snowflakes_emitter.createParticles(40);

				--coolers_;
				panel_.SetCoolers(coolers_);
				heat_ = 0.f;
				overheated_ = false;
				panel_.SetOverheat(false);
			}
			else
			{
				SoundSystem::playSound("disabled.ogg");
			}
			break;
		case Action::USE_MISSILE:
			if (missiles_ > 0 && m_missile_launcher.isReady())
			{
				--missiles_;
				panel_.SetMissiles(missiles_);
				m_missile_launcher.shoot(0);
			}
			else
			{
				SoundSystem::playSound("disabled.ogg");
			}
			break;
		case Action::USE_LASER:
			if (overheated_)
			{
				SoundSystem::playSound("disabled.ogg", 0.9);
			}
			break;
		case Action::NONE:
			return;
		default:
			// Continue because others actions may belong to the Konami Code
			break;
	}

	// Konami code
	if (action == m_konami_code[m_current_konami_index])
	{
		++m_current_konami_index;
		if (m_current_konami_index == KONAMI_CODE_LENGTH)
		{
			applyKonamiCode();
			m_current_konami_index = 0; // Reset
		}
	}
	else
	{
		m_current_konami_index = 0;
	}
}


void Player::onActionUp(Action::ID action)
{
	switch (action)
	{
		case Action::UP:
		case Action::DOWN:
			m_animator.setAnimation(*this, m_animation_normal);
			break;
		default:
			break;
	}
}


void Player::onUpdate(float frametime)
{
	static const EntityManager& manager = EntityManager::getInstance();

	// Animation
	m_animator.updateSubRect(*this, frametime);

	// Weapons
	if (!overheated_)
	{
		float h = 0.f;
		if (Input::isPressed(Action::USE_LASER))
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
			overheatAudioHint();
	}

	// Compute position
	sf::Vector2f pos = getPosition();
	float delta = m_speed * frametime;

	if (Input::isPressed(Action::UP))
		pos.y -= delta;

	if (Input::isPressed(Action::DOWN))
		pos.y += delta;

	if (Input::isPressed(Action::LEFT))
		pos.x -= delta;

	if (Input::isPressed(Action::RIGHT))
		pos.x += delta;

	// Ensure new position is within screen's limits
	const int X_BOUND = manager.getWidth() - getWidth();
	const int Y_BOUND = manager.getHeight() - getHeight();

	if (pos.y < 0)
		pos.y = 0;
	else if (pos.y > Y_BOUND)
		pos.y = Y_BOUND;

	if (pos.x < 0)
		pos.x = 0;
	else if (pos.x > X_BOUND)
		pos.x = X_BOUND;

	// Apply new position
	setPosition(pos);
	m_smoke_emitter.setPosition(getX(), getY() + getHeight() / 2);
	m_shield_emitter.setPosition(getCenter());

	// Regenerate shield
	if (shield_ < shield_max_)
	{
		shield_timer_ += frametime;
		if (shield_timer_ >= SHIELD_RECOVERY_DELAY)
		{
			setShield(shield_ + 1);
			shield_timer_ = 0.f;
		}
	}

	// Cooling heatsink
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

	// Decrase powerups timers
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

	updateDamageFlash(frametime);
}


void Player::takeDamage(int damage)
{
	if (damage == 0)
		return;

	if (shield_ > 0)
	{
		shield_ -= damage;
		if (shield_ < 0)
			shield_ = 0;

		SoundSystem::playSound("shield-damage.ogg");
		m_shield_emitter.createParticles(shield_);
		panel_.SetShield(shield_);
	}
	else
	{
		Damageable::takeDamage(damage);
		SoundSystem::playSound("ship-damage.ogg");
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
				m_speed *= BONUS_SPEED_FACTOR;
				m_smoke_emitter.createParticles(120);
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			panel_.ActiveSpeedPowerUp(bonus_[T_SPEED]);
			break;
		// immediate bonus
		case PowerUp::SUPER_BANANA:
			m_powerup_emitter.setPosition(getCenter());
			m_powerup_emitter.createParticles(50);
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
	SoundSystem::playSound("power-up.ogg");
}


void Player::onDestroy()
{
	setColor(sf::Color::White); // clear red flash
	m_animator.setAnimation(*this, EntityManager::getInstance().getAnimation("player-destroyed"));
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
			m_speed /= BONUS_SPEED_FACTOR;
			m_smoke_emitter.clearParticles();
			break;
		default:
			break;
	}
	bonus_[tbonus] = 0;
}


void Player::setShield(int count)
{
	shield_ = count;
	// Update particles
	m_shield_emitter.createParticles(count);

	// Update panel count
	panel_.SetShield(count);
}


void Player::applyKonamiCode()
{
	m_konami_code_activated = true;

	// Set max hp
	setHP(hp_max_);
	panel_.SetShipHP(hp_max_);

	// Set max shield
	setShield(shield_max_);

	coolers_ = 42;
	panel_.SetCoolers(42);
	missiles_ = 42;
	panel_.SetMissiles(42);

	m_weapon.setMultiply(3);
	m_missile_launcher.setMultiply(3);

	MessageSystem::write("KONAMI CODE ON", getPosition());
	m_powerup_emitter.setPosition(getCenter());
	m_powerup_emitter.createParticles(50);
}

// Shield::Emitter -------------------------------------------------------------

void Player::ShieldEmitter::createParticles(size_t count)
{
	// Remove all previous particles
	clearParticles();

	float angle = 2 * math::PI / count;

	for (size_t i = 0; i < count; ++i)
	{
		ParticleSystem::Particle p(*this);
		resetParticle(p);
		p.angle = angle * (i + 1);
		ParticleSystem::getInstance().addParticle(p);
	}
}


void Player::ShieldEmitter::onParticleUpdated(ParticleSystem::Particle& particle, float frametime) const
{
	sf::Vector2f circle_center = getPosition();
	// Rotation de 2 * PI par seconde
	particle.angle += std::fmod((2 * math::PI * frametime), 2 * math::PI);
	const float SHIELD_RADIUS = 42.f;
	particle.position.x = circle_center.x + (SHIELD_RADIUS) * std::cos(particle.angle);
	particle.position.y = circle_center.y - (SHIELD_RADIUS) * std::sin(particle.angle);
}
