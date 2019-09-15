#include "Boss1.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "entities/Missile.hpp"
#include "core/Resources.hpp"

Boss1::Boss1():
    m_angle(math::PI),
    m_timer(0),
    m_state(IDLE)
{
    setTexture(Resources::getTexture("entities/bandit-purple-mk3.png"));
    setHP(120);

    m_weapon.init("laser-pink");
    m_weapon.setOwner(this);
    m_weapon.setPosition(74, 42);
    m_weapon.setMultiply(3);

    m_missile.init("missile");
    m_missile.setOwner(this);
    m_missile.setPosition(80, 42);
}


void Boss1::onUpdate(float frametime)
{
    m_timer += frametime;
    switch (m_state) {
        case IDLE:
            move(-40.f * frametime, 0);
            if (m_timer > 5.f) {
                m_timer = 0;
                m_state = CIRCLE;
                m_circleCenter = getPosition();
            }
            break;
        case CIRCLE: {
            sf::Vector2f pos(m_circleCenter);
            m_angle += math::PI / 2.f * frametime;
            pos.x += 80 * std::cos(m_angle);
            pos.y -= 80 * std::sin(m_angle);
            setPosition(pos);
            if (m_timer > 7.f) {
                m_timer = 0;
                m_state = ATTACK;
            }
            break;
        }
        case ATTACK:
            m_weapon.shoot(-math::PI);
            if (m_timer > 4.f) {
                m_timer = 0;
                m_state = MISSILE;
            }
            break;
        case MISSILE:
            m_missile.shoot<Missile>(EntityManager::getInstance().getPlayer()->getPosition());
            if (m_timer > 3.f) {
                m_timer = 0;
                m_state = CIRCLE;
            }
            break;
    }

    updateDamageFlash(frametime);
}
