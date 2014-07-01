#include "Player.hpp"
#include "Missile.hpp"
#include "EntityManager.hpp"
#include "core/UserSettings.hpp"
#include "core/MessageSystem.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "items/ItemManager.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"

#define BONUS_SPEED_FACTOR 1.5

// time to wait from overheat to heat 0 (seconds)
#define COOLING_DELAY              10

#define TIMED_BONUS_DURATION        10
const int MAX_MISSILES = 5;
const int MAX_ICECUBES = 5;


Player::Player():
	m_panel(ControlPanel::getInstance()),
	m_overheat(false),
	m_heat(0.f),
	m_max_heat(0.f),
	m_shield(0),
	m_max_shield(0),
	m_max_hp(0),
	m_speed(0.f),
	m_missiles(0),
	m_icecubes(0),
	// Preload animations
	m_animation_up(EntityManager::getInstance().getAnimation("player-up")),
	m_animation_down(EntityManager::getInstance().getAnimation("player-down")),
	m_animation_normal(EntityManager::getInstance().getAnimation("player")),
	m_score(0)
{
	setTeam(Entity::GOOD);
	setHP(1);

	// Init sprite texture
	m_animator.setAnimation(*this, m_animation_normal);

	// Init weapons
	int xweapon = 54, yweapon = 24;
	m_weapon.init("laser-red");
	m_weapon.setOwner(this);
	m_weapon.setPosition(xweapon, yweapon);

	m_missile_launcher.init("missile");
	m_missile_launcher.setOwner(this);
	m_missile_launcher.setPosition(xweapon, yweapon);

	// Init timed bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		bonus_[i] = 0.f;
	}

	// Init control panel
	m_panel.setIcecubes(m_icecubes);
	m_panel.setMissiles(m_missiles);
	m_panel.setOverheat(m_overheat);
	m_panel.ActiveSpeedPowerUp(0);
	m_panel.ActiveAttackPowerUp(0, PowerUp::DOUBLE_SHOT);

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
	const ItemManager& items = ItemManager::getInstance();

	// Reset score
	m_score = 0;
	m_panel.setScore(0);

	// Equip Shield item
	items.getItem(Item::SHIELD, UserSettings::getItemLevel(Item::SHIELD)).equip(*this);
	setShield(m_shield); // Keep previous shield value

	// Equip Hull item and repair spaceship
	items.getItem(Item::HULL, UserSettings::getItemLevel(Item::HULL)).equip(*this);
	setHP(m_max_hp);
	m_panel.setHP(m_max_hp);

	// Equip Engine item
	items.getItem(Item::ENGINE, UserSettings::getItemLevel(Item::ENGINE)).equip(*this);

	// Equip Heatsink item and for cooldown
	items.getItem(Item::HEATSINK, UserSettings::getItemLevel(Item::HEATSINK)).equip(*this);
	m_heat = 0;
	m_panel.setHeat(m_heat);
	m_overheat = false;
	m_panel.setOverheat(false);

	// Equip Weapon item
	items.getItem(Item::WEAPON, UserSettings::getItemLevel(Item::WEAPON)).equip(*this);
}


int Player::getScore() const
{
	return m_score;
}


void Player::updateScore(int diff)
{
	m_score += diff;
	m_panel.setScore(m_score);
}


void Player::overheatAudioHint() const
{
	static float thresholds[] = {.50f, .60f, .70f, .80f, .90f};
	static int thresholds_count = sizeof (thresholds) / sizeof (float);
	static int current_index = 0;

	// Get heat value between 0 and 1
	float heat_percent = m_heat / m_max_heat;
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
			if (m_icecubes > 0)
			{
				// Play sound effect and launch particles
				SoundSystem::playSound("cooler.ogg");
				m_snowflakes_emitter.setPosition(getCenter());
				m_snowflakes_emitter.createParticles(40);

				// Reset heat
				m_panel.setIcecubes(--m_icecubes);
				m_heat = 0.f;
				m_overheat = false;
				m_panel.setOverheat(false);
			}
			else
			{
				SoundSystem::playSound("disabled.ogg");
			}
			break;
		case Action::USE_MISSILE:
			if (m_missiles > 0 && m_missile_launcher.isReady())
			{
				m_panel.setMissiles(--m_missiles);
				m_missile_launcher.shoot<Missile>(0);
			}
			else
			{
				SoundSystem::playSound("disabled.ogg");
			}
			break;
		case Action::USE_LASER:
			if (m_overheat)
			{
				SoundSystem::playSound("disabled.ogg");
			}
			break;
		default:
			break;
	}

	// Detect Konami code sequence
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
	if (!m_overheat)
	{
		float h = 0.f;
		if (Input::isPressed(Action::USE_LASER))
		{
			h += m_weapon.shoot(0);
		}

		m_heat += h;
		if (m_heat >= m_max_heat)
		{
			m_overheat = true;
			m_panel.setOverheat(true);
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

	// Cooling heatsink
	if (m_heat > 0.f)
	{
		m_heat -= frametime * m_max_heat / COOLING_DELAY;
		if (m_heat <= 0.f)
		{
			m_heat = 0.f;
			if (m_overheat )
			{
				m_overheat = false;
				m_panel.setOverheat(false);
			}
		}
	}
	m_panel.setHeat(m_heat);

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

	if (m_shield > 0)
	{
		m_shield -= damage;
		if (m_shield < 0)
			m_shield = 0;

		SoundSystem::playSound("shield-damage.ogg");
		m_shield_emitter.createParticles(m_shield);
		m_panel.setShield(m_shield);
	}
	else
	{
		Damageable::takeDamage(damage);
		SoundSystem::playSound("ship-damage.ogg");
		m_panel.setHP(getHP());
	}
}


void Player::onCollision(PowerUp& powerup)
{
	switch (powerup.getType())
	{
		// timed bonus
		case PowerUp::DOUBLE_SHOT:
			if (bonus_[T_DOUBLESHOT] == 0)
				m_weapon.setMultiply(2);

			bonus_[T_TRISHOT] = 0;
			bonus_[T_DOUBLESHOT] += TIMED_BONUS_DURATION;
			m_panel.ActiveAttackPowerUp(bonus_[T_DOUBLESHOT], powerup.getType());
			break;

		case PowerUp::TRIPLE_SHOT:
			if (bonus_[T_TRISHOT] == 0)
				m_weapon.setMultiply(3);

			bonus_[T_DOUBLESHOT] = 0;
			bonus_[T_TRISHOT] += TIMED_BONUS_DURATION;
			m_panel.ActiveAttackPowerUp(bonus_[T_TRISHOT], powerup.getType());
			break;

		case PowerUp::SPEED:
			if (bonus_[T_SPEED] == 0)
			{
				m_speed *= BONUS_SPEED_FACTOR;
				m_smoke_emitter.createParticles(120);
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			m_panel.ActiveSpeedPowerUp(bonus_[T_SPEED]);
			break;

		// immediate bonus
		case PowerUp::REPAIR:
			if (getHP() < m_max_hp)
				m_panel.setHP(updateHP(1));
			break;

		case PowerUp::FULL_REPAIR:
			setHP(m_max_hp);
			m_panel.setHP(m_max_hp);
			m_powerup_emitter.setPosition(getCenter());
			m_powerup_emitter.createParticles(50);
			break;

		case PowerUp::SHIELD:
			if (m_shield < m_max_shield)
				setShield(m_shield + 1);
			break;

		case PowerUp::FULL_SHIELD:
			setShield(m_max_shield);
			m_panel.setShield(m_max_shield);
			m_powerup_emitter.setPosition(getCenter());
			m_powerup_emitter.createParticles(50);
			break;

		case PowerUp::ICECUBE:
			if (m_icecubes < MAX_ICECUBES)
				m_panel.setIcecubes(++m_icecubes);
			break;

		case PowerUp::MISSILE:
			if (m_missiles < MAX_MISSILES)
				m_panel.setMissiles(++m_missiles);
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
	m_shield = count;
	// Update particles
	m_shield_emitter.createParticles(count);

	// Update panel count
	m_panel.setShield(count);
}


void Player::setMaxHeat(int heat)
{
	m_max_heat = heat;
	m_panel.setMaxHeat(heat);
}


void Player::setMaxHP(int hp)
{
	m_max_hp = hp;
	m_panel.setMaxHP(hp);
}


void Player::setSpeed(float speed)
{
	m_speed = speed;
}


void Player::setMaxShield(int shield)
{
	m_max_shield = shield;
	m_panel.setMaxShield(shield);
}


Weapon& Player::getLaser()
{
	return m_weapon;
}


void Player::applyKonamiCode()
{
	m_konami_code_activated = true;

	// Set max hp
	setMaxHP(10);
	setHP(m_max_hp);
	m_panel.setHP(m_max_hp);

	// Set max shield
	setMaxShield(10);
	setShield(m_max_shield);

	m_icecubes = 42;
	m_panel.setIcecubes(42);
	m_missiles = 42;
	m_panel.setMissiles(42);

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
