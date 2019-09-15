#include "Player.hpp"
#include "Missile.hpp"
#include "Explosion.hpp"
#include "EntityManager.hpp"
#include "core/Services.hpp"
#include "core/Factory.hpp"
#include "core/UserSettings.hpp"
#include "core/MessageSystem.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"
#include "utils/SFML_Helper.hpp"

// Speed multiplier with speed power-up
static constexpr float POWERUP_SPEED_FACTOR = 1.5;

// Time to wait from overheat to heat 0 (seconds)
static constexpr int COOLING_DELAY = 10;

// Radius in pixels for shield particles moving in a circle motion around player
static constexpr float SHIELD_RADIUS = 42.f;

// Effect duration for all time-based power ups
static constexpr int TIMED_POWERUP_DURATION = 10;

static constexpr int MAX_MISSILES = 5;
static constexpr int MAX_ICECUBES = 5;


Player::Player():
    m_overheat(false),
    m_heat(0.f),
    m_max_heat(0.f),
    m_shield(0),
    m_max_shield(0),
    m_max_hp(0),
    m_speed(0.f),
    m_missiles(0),
    m_icecubes(0),
    m_score(0),
    m_shieldEmitter(Services::getParticleSystem()),
    m_engineEmitter(Services::getParticleSystem()),
    m_smokeEmitter(Services::getParticleSystem()),
    m_snowflakesEmitter(Services::getParticleSystem()),
    m_powerupEmitter(Services::getParticleSystem()),
    m_hud(Services::getHUD())
{
    setTeam(Entity::GOOD);
    setHP(1);

    // Init sprite texture
    m_animator.setAnimation(*this, Services::getFactory().getAnimation("player"));

    // Init weapons
    int xweapon = 54, yweapon = 24;
    m_weapon.init("laser-red");
    m_weapon.setOwner(this);
    m_weapon.setPosition(xweapon, yweapon);

    m_missileLauncher.init("missile");
    m_missileLauncher.setOwner(this);
    m_missileLauncher.setPosition(xweapon, yweapon);

    // Init timed bonus
    for (int i = 0; i < TIMED_POWERUP_COUNT; ++i) {
        m_powerUps[i] = 0.f;
    }

    // Init control panel
    m_hud.setIcecubes(m_icecubes);
    m_hud.setMissiles(m_missiles);
    m_hud.setOverheat(m_overheat);
    m_hud.activeSpeedPowerUp(0);
    m_hud.activeAttackPowerUp(0, PowerUp::DOUBLE_SHOT);

    // Init Konami code sequence
    m_konamiCode[0] = Action::UP;
    m_konamiCode[1] = Action::UP;
    m_konamiCode[2] = Action::DOWN;
    m_konamiCode[3] = Action::DOWN;
    m_konamiCode[4] = Action::LEFT;
    m_konamiCode[5] = Action::RIGHT;
    m_konamiCode[6] = Action::LEFT;
    m_konamiCode[7] = Action::RIGHT;
    m_konamiCode[8] = Action::USE_MISSILE;
    m_konamiCode[9] = Action::USE_LASER;
    m_currentKonamiIndex = 0;
    m_konamiCodeActivated = false;

    // Init particles emitters
    m_shieldEmitter.setTextureRect(sf::IntRect(40, 0, 8, 8));
    m_shieldEmitter.setLifetime(0);

    m_smokeEmitter.setTextureRect(sf::IntRect(0, 0, 16, 16));
    m_smokeEmitter.setLooping(true);
    m_smokeEmitter.setSpeed(50, 25);
    m_smokeEmitter.setLifetime(1.5);
    m_smokeEmitter.setAngle(-math::PI, 0.5);
    m_smokeEmitter.setScale(0.5, 1.5);

    m_snowflakesEmitter.setTextureRect(sf::IntRect(16, 0, 16, 16));
    m_snowflakesEmitter.setScale(0.2, 1.5);

    m_engineEmitter.setTextureRect({32, 9, 3, 3});
    m_engineEmitter.setAngle(-math::PI, 0.5);
    m_engineEmitter.setParticleColor(sf::Color::Yellow, sf::Color(255, 0, 0, 0));
    m_engineEmitter.setParticleColor(sf::Color::Cyan, sf::Color(0, 0, 255, 0));
    m_engineEmitter.setLooping(true);
    m_engineEmitter.setScale(2, 1);
    m_engineEmitter.setLifetime(0.5f);
    m_engineEmitter.setSpeed(30, 0);

    m_powerupEmitter.setTextureRect(sf::IntRect(32, 0, 8, 8));
}


Player::~Player()
{
    m_engineEmitter.clearParticles();
    m_smokeEmitter.clearParticles();
    m_shieldEmitter.clearParticles();
}


void Player::onInit()
{
    // Reset score
    m_score = 0;
    m_hud.setScore(0);

    // Equip Shield item
    Factory& factory = Services::getFactory();
    factory.getItem(Item::SHIELD, UserSettings::getItemLevel(Item::SHIELD)).equip(*this);
    setShield(m_shield); // Keep previous shield value

    // Equip Hull item and repair spaceship
    factory.getItem(Item::HULL, UserSettings::getItemLevel(Item::HULL)).equip(*this);
    setHP(m_max_hp);
    m_hud.setHP(m_max_hp);

    // Equip Engine item
    factory.getItem(Item::ENGINE, UserSettings::getItemLevel(Item::ENGINE)).equip(*this);

    // Equip Heatsink item and for cooldown
    factory.getItem(Item::HEATSINK, UserSettings::getItemLevel(Item::HEATSINK)).equip(*this);
    m_heat = 0;
    m_hud.setHeat(m_heat);
    m_overheat = false;
    m_hud.setOverheat(false);

    // Equip Weapon item
    factory.getItem(Item::WEAPON, UserSettings::getItemLevel(Item::WEAPON)).equip(*this);

    m_engineEmitter.createParticles(100);
}


int Player::getScore() const
{
    return m_score;
}


void Player::updateScore(int diff)
{
    m_score += diff;
    m_hud.setScore(m_score);
}


void Player::overheatAudioHint() const
{
    static float thresholds[] = {.50f, .60f, .70f, .80f, .90f};
    static int thresholds_count = sizeof (thresholds) / sizeof (float);
    static int current_index = 0;

    // Get heat value between 0 and 1
    float heat_percent = m_heat / m_max_heat;
    if (current_index < thresholds_count && heat_percent > thresholds[current_index]) {
        // New threshold reached
        Services::getSoundSystem().playSound("overheat.ogg", 0.4 + thresholds[current_index]);
        ++current_index;
    } else if (current_index > 0 && heat_percent < thresholds[current_index -1]) {
        --current_index;
    }
}


void Player::onActionDown(Action::ID action)
{
    switch (action) {
        case Action::UP:
            m_animator.setAnimation(*this, Services::getFactory().getAnimation("player-up"));
            break;
        case Action::DOWN:
            m_animator.setAnimation(*this, Services::getFactory().getAnimation("player-down"));
            break;
        case Action::USE_COOLER:
            if (m_icecubes > 0) {
                // Play sound effect and launch particles
                Services::getSoundSystem().playSound("cooler.ogg");
                m_snowflakesEmitter.setPosition(getCenter());
                m_snowflakesEmitter.createParticles(40);

                // Reset heat
                m_hud.setIcecubes(--m_icecubes);
                m_heat = 0.f;
                m_overheat = false;
                m_hud.setOverheat(false);
            } else {
                Services::getSoundSystem().playSound("disabled.ogg");
            }
            break;
        case Action::USE_MISSILE:
            if (m_missiles > 0 && m_missileLauncher.isReady()) {
                m_hud.setMissiles(--m_missiles);
                m_missileLauncher.shoot<Missile>(0);
            } else {
                Services::getSoundSystem().playSound("disabled.ogg");
            }
            break;
        case Action::USE_LASER:
            if (m_overheat) {
                Services::getSoundSystem().playSound("disabled.ogg");
            }
            break;
        default:
            break;
    }

    // Detect Konami code sequence
    if (action == m_konamiCode[m_currentKonamiIndex]) {
        ++m_currentKonamiIndex;
        if (m_currentKonamiIndex == KONAMI_CODE_LENGTH) {
            toggleKonamiCode();
            m_currentKonamiIndex = 0; // Reset
        }
    } else {
        m_currentKonamiIndex = 0;
    }
}


void Player::onActionUp(Action::ID action)
{
    switch (action) {
        case Action::UP:
        case Action::DOWN:
            m_animator.setAnimation(*this, Services::getFactory().getAnimation("player"));
            break;
        default:
            break;
    }
}


void Player::onUpdate(float frametime)
{
    static const EntityManager& manager = EntityManager::getInstance();

    // Weapons
    if (!m_overheat) {
        float h = 0.f;
        if (Input::isPressed(Action::USE_LASER)) {
            h += m_weapon.shoot(0);
        }

        m_heat += h;
        if (m_heat >= m_max_heat) {
            m_overheat = true;
            m_hud.setOverheat(true);
            Services::getMessageSystem().write(_t("panel.overheat"), getPosition());
        }
        if (h > 0) {
            overheatAudioHint();
        }
    }

    // Compute position
    sf::Vector2f pos = getPosition();
    const float delta = m_speed * frametime;

    if (Input::isPressed(Action::UP)) {
        pos.y -= delta;
    }
    if (Input::isPressed(Action::DOWN)) {
        pos.y += delta;
    }
    if (Input::isPressed(Action::LEFT)) {
        pos.x -= delta;
    }
    if (Input::isPressed(Action::RIGHT)) {
        pos.x += delta;
    }

    const sf::FloatRect& rect = manager.getViewZone();

    // Ensure new position is within screen's limits
    const float maxX = rect.left + rect.width - getWidth();
    const float maxY = rect.top + rect.height - getHeight();

    pos.x = math::clamp(pos.x, rect.left, maxX);
    pos.y = math::clamp(pos.y, rect.top, maxY);

    // Apply new position
    setPosition(pos);
    m_smokeEmitter.setPosition(getX(), getY() + 22);
    m_engineEmitter.setPosition(getX(), getY() + 22);
    m_shieldEmitter.setPosition(getCenter());

    // Cooling heatsink
    if (m_heat > 0.f) {
        m_heat -= frametime * m_max_heat / COOLING_DELAY;
        if (m_heat <= 0.f) {
            m_heat = 0.f;
            if (m_overheat ) {
                m_overheat = false;
                m_hud.setOverheat(false);
            }
        }
    }
    m_hud.setHeat(m_heat);

    // Decrase powerups timers
    for (int i = 0; i < TIMED_POWERUP_COUNT; ++i) {
        if (m_powerUps[i] > 0) {
            m_powerUps[i] -= frametime;
            if (m_powerUps[i] <= 0) {
                m_powerUps[i] = 0;
                DisableTimedPowerUp((TimedPowerUp) i);
            }
        }
    }

    updateDamageFlash(frametime);
}


void Player::takeDamage(int damage)
{
    if (damage == 0 || m_konamiCodeActivated) {
        return;
    }

    if (m_shield > 0) {
        m_shield -= damage;
        if (m_shield < 0) {
            m_shield = 0;
        }
        Services::getSoundSystem().playSound("shield-damage.ogg");
        m_shieldEmitter.createParticles(m_shield);
        m_hud.setShield(m_shield);
    } else {
        Damageable::takeDamage(damage);
        Services::getSoundSystem().playSound("ship-damage.ogg");
        m_hud.setHP(getHP());

        if (getHP() == 1) {
            m_smokeEmitter.createParticles(100);
        }
    }
}


void Player::onCollision(PowerUp& powerup)
{
    switch (powerup.getType()) {
        // timed bonus
        case PowerUp::DOUBLE_SHOT:
            if (m_powerUps[T_DOUBLESHOT] == 0) {
                m_weapon.setMultiply(2);
            }
            m_powerUps[T_TRISHOT] = 0;
            m_powerUps[T_DOUBLESHOT] += TIMED_POWERUP_DURATION;
            m_hud.activeAttackPowerUp(m_powerUps[T_DOUBLESHOT], powerup.getType());
            break;

        case PowerUp::TRIPLE_SHOT:
            if (m_powerUps[T_TRISHOT] == 0) {
                m_weapon.setMultiply(3);
            }
            m_powerUps[T_DOUBLESHOT] = 0;
            m_powerUps[T_TRISHOT] += TIMED_POWERUP_DURATION;
            m_hud.activeAttackPowerUp(m_powerUps[T_TRISHOT], powerup.getType());
            break;

        case PowerUp::SPEED:
            if (m_powerUps[T_SPEED] == 0) {
                m_speed *= POWERUP_SPEED_FACTOR;
                m_engineEmitter.setParticleColor(sf::Color::Cyan, sf::Color(0, 0, 255, 0));
            }
            m_powerUps[T_SPEED] += TIMED_POWERUP_DURATION;
            m_hud.activeSpeedPowerUp(m_powerUps[T_SPEED]);
            break;

        // immediate bonus
        case PowerUp::REPAIR:
            if (getHP() < m_max_hp) {
                m_hud.setHP(updateHP(1));
            }
            m_smokeEmitter.clearParticles();
            break;

        case PowerUp::FULL_REPAIR:
            setHP(m_max_hp);
            m_hud.setHP(m_max_hp);
            m_powerupEmitter.setPosition(getCenter());
            m_powerupEmitter.createParticles(50);
            m_smokeEmitter.clearParticles();
            break;

        case PowerUp::SHIELD:
            if (m_shield < m_max_shield) {
                setShield(m_shield + 1);
            }
            break;

        case PowerUp::FULL_SHIELD:
            setShield(m_max_shield);
            m_hud.setShield(m_max_shield);
            m_powerupEmitter.setPosition(getCenter());
            m_powerupEmitter.createParticles(50);
            break;

        case PowerUp::ICECUBE:
            if (m_icecubes < MAX_ICECUBES) {
                m_hud.setIcecubes(++m_icecubes);
            }
            break;

        case PowerUp::MISSILE:
            if (m_missiles < MAX_MISSILES) {
                m_hud.setMissiles(++m_missiles);
            }
            break;

        default:
            break;
    }

    powerup.kill();
    Services::getMessageSystem().write(powerup.getDescription(), powerup.getPosition());
    Services::getSoundSystem().playSound("power-up.ogg");
}


void Player::onTileCollision()
{
    if (!m_konamiCodeActivated) {
        Damageable::onTileCollision();
    }
}


void Player::onDestroy()
{
    Explosion* explosion = new Explosion();
    explosion->setPosition(getCenter());
    EntityManager::getInstance().addEntity(explosion);

    setColor(sf::Color::White); // clear red flash
    m_animator.setAnimation(*this, Services::getFactory().getAnimation("player-destroyed"));
}


void Player::DisableTimedPowerUp(TimedPowerUp tbonus)
{
    switch (tbonus) {
        case T_DOUBLESHOT:
        case T_TRISHOT:
            m_weapon.setMultiply(1);
            break;
        case T_SPEED:
            m_speed /= POWERUP_SPEED_FACTOR;
            m_engineEmitter.setParticleColor(sf::Color::Yellow, sf::Color(255, 0, 0, 0));
            break;
        default:
            break;
    }
    m_powerUps[tbonus] = 0;
}


void Player::setShield(int count)
{
    m_shield = count;
    // Update particles
    m_shieldEmitter.createParticles(count);

    // Update panel count
    m_hud.setShield(count);
}


void Player::setMaxHeat(int heat)
{
    m_max_heat = heat;
    m_hud.setMaxHeat(heat);
}


void Player::setMaxHP(int hp)
{
    m_max_hp = hp;
    m_hud.setMaxHP(hp);
}


void Player::setSpeed(float speed)
{
    m_speed = speed;
}


void Player::setMaxShield(int shield)
{
    m_max_shield = shield;
    m_hud.setMaxShield(shield);
}


Weapon& Player::getLaser()
{
    return m_weapon;
}


void Player::toggleKonamiCode()
{
    m_konamiCodeActivated = !m_konamiCodeActivated;
    EntityManager::getInstance().toggleFastScrolling();
    if (m_konamiCodeActivated) {
        // Set max hp
        setHP(m_max_hp);
        m_hud.setHP(m_max_hp);

        // Set max shield
        setShield(m_max_shield);

        m_icecubes = 42;
        m_hud.setIcecubes(42);
        m_missiles = 42;
        m_hud.setMissiles(42);

        m_weapon.setMultiply(3);
        m_missileLauncher.setMultiply(3);

        Services::getMessageSystem().write("KONAMI CODE ON", getPosition());
        m_powerupEmitter.setPosition(getCenter());
        m_powerupEmitter.createParticles(50);
    } else {
        Services::getMessageSystem().write("KONAMI CODE OFF", getPosition());
    }
}

// Shield::Emitter -------------------------------------------------------------

void Player::ShieldEmitter::createParticles(size_t count)
{
    // Remove all previous particles
    clearParticles();

    float angle = 2 * math::PI / count;

    for (size_t i = 0; i < count; ++i) {
        Particle p(*this);
        resetParticle(p);
        p.angle = angle * (i + 1);
        Services::getParticleSystem().addParticle(p);
    }
}

void Player::ShieldEmitter::onParticleUpdated(Particle& particle, float frametime) const
{
    sf::Vector2f circle_center = getPosition();
    // Rotation de 2 * PI par seconde
    particle.angle += std::fmod((2 * math::PI * frametime), 2 * math::PI);
    particle.position.x = circle_center.x + (SHIELD_RADIUS) * std::cos(particle.angle);
    particle.position.y = circle_center.y - (SHIELD_RADIUS) * std::sin(particle.angle);
}
