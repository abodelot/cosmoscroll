#include "Spaceship.hpp"
#include "EntityManager.hpp"
#include "Player.hpp"
#include "utils/Math.hpp"
#include "core/Services.hpp"
#include "core/MessageSystem.hpp"

// bonus freq = 1 / DROP_LUCK
#define DROP_LUCK 1

// sinus movement settings
static constexpr float SINUS_AMPLITUDE = 60;
static constexpr float SINUS_FREQUENCE = 0.02;

// circle movement settings
static constexpr float CIRCLE_RADIUS = 60;
static constexpr float CIRCLE_RAD_PER_SEC = math::PI * 0.8;

Spaceship::Spaceship(const Animation& animation, int hp, int speed):
    m_attack(NONE),
    m_movement(LINE),
    m_points(1),
    m_target(nullptr),
    m_speed(speed),
    // For CIRCLE movement, start the angle at PI (top of circle), so spaceship remains on its
    // inital Y axis, and starts moving immediately to the left (in a counter-clock wise rotation).
    m_angle(math::PI),
    m_engineEmitter(Services::getParticleSystem()),
    m_engineEmitterEnabled(false)
{
    setTexture(animation.getTexture());
    setTeam(Entity::BAD);
    setHP(hp);
    m_animator.setAnimation(*this, animation);

    m_weapon.setOwner(this);
}


Spaceship::~Spaceship()
{
}


void Spaceship::onInit()
{
    m_spawnPosition = getPosition();
    if (m_movement == SINUS) {
        // Adjust initial position to ensure spaceship will always be within the screen boundaries
        float y = getY();
        if (y < SINUS_AMPLITUDE) {
            m_spawnPosition.y = SINUS_AMPLITUDE;
        } else {
            // Compute maximal Y position according to the wave's amplitude
            float maxY = EntityManager::getInstance().getSize().y - SINUS_AMPLITUDE - getHeight();
            if (y > maxY)
                m_spawnPosition.y = maxY;
        }

        setPosition(m_spawnPosition);
    }

    m_target = EntityManager::getInstance().getPlayer();

    if (m_engineEmitterEnabled) {
        m_engineEmitter.setTextureRect({32, 9, 3, 3});
        m_engineEmitter.setAngle(math::PI * 2, 0.1);
        m_engineEmitter.setParticleColor(sf::Color::Yellow, sf::Color(255, 0, 0, 0));
        m_engineEmitter.setLooping(true);
        m_engineEmitter.setScale(2, 1);
        m_engineEmitter.setLifetime(0.2);
        m_engineEmitter.setSpeed(150, 10);
        m_engineEmitter.createParticles(40);
    }
}


void Spaceship::setMovementPattern(MovementPattern movement)
{
    m_movement = movement;
}


void Spaceship::setAttackPattern(AttackPattern attack)
{
    m_attack = attack;
}


Spaceship* Spaceship::clone() const
{
    Spaceship* ship = new Spaceship(*this);
    ship->setPoints(getPoints()); // ???
    ship->m_weapon.setOwner(ship);
    return ship;
}


void Spaceship::onUpdate(float frametime)
{
    m_animator.updateSubRect(*this, frametime);

    // Apply movement pattern
    float delta = m_speed * frametime;
    switch (m_movement) {
        case LINE:
            move(-delta, 0);
            break;
        case MAGNET: {
            sf::Vector2f pos = getPosition();

            // Translate position towards target's position
            math::translate(pos, math::angle(pos, m_target->getPosition()), delta);
            setPosition(pos);
            break;
        }
        case SINUS: {
            sf::Vector2f pos = getPosition();
            pos.x -= delta;
            pos.y = std::sin(pos.x * SINUS_FREQUENCE) * SINUS_AMPLITUDE + m_spawnPosition.y;
            setPosition(pos);
            break;
        }
        case CIRCLE: {
            sf::Vector2f pos(m_spawnPosition);
            m_angle += CIRCLE_RAD_PER_SEC * frametime;
            pos.x += CIRCLE_RADIUS * std::cos(m_angle);
            pos.y -= CIRCLE_RADIUS * std::sin(m_angle);
            setPosition(pos);
            break;
        }
    }

    switch (m_attack) {
        case AUTO_AIM:
            m_weapon.shoot(m_target->getCenter());
            break;
        case ON_SIGHT:
            m_weapon.shoot(-math::PI);
            break;
        case NONE:
            break;
    }

    updateDamageFlash(frametime);

    if (m_engineEmitterEnabled)
    {
        m_engineEmitter.setPosition(getPosition() + m_engineOffset);
    }
}


void Spaceship::onDestroy()
{
    Damageable::onDestroy();
    if (math::rand(1, DROP_LUCK) == 1) {
        PowerUp::dropRandom(getPosition());
    }

    EntityManager::getInstance().getPlayer()->updateScore(m_points);
    Services::getMessageSystem().write(
        "+" + std::to_string(m_points),
        getPosition(),
        sf::Color(255, 128, 0));
}


Weapon& Spaceship::getWeapon()
{
    return m_weapon;
}


void Spaceship::enableEngineEffect(const sf::Vector2f& offset)
{
    m_engineEmitterEnabled = true;
    m_engineOffset = offset;
}


void Spaceship::setPoints(int points)
{
    m_points = points;
}


int Spaceship::getPoints() const
{
    return m_points;
}
