#include "TentaculatBoss.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/Math.hpp"
#include "core/Services.hpp"


TentaculatBoss::TentaculatBoss():
    m_state(LURK),
    m_timer(0.f),
    m_speed(0.f, 120.f),
    m_target(nullptr)
{
    setTeam(Entity::BAD);
    setHP(300);

    // init weapon
    m_weapon.init("laser-pink");
    m_weapon.setOwner(this);
    m_weapon.setPosition(74, 42);
    m_weapon.setMultiply(3);

   m_animator.setAnimation(*this, Services::getFactory().getAnimation("boss-tentacles"));
}


void TentaculatBoss::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();
}


void TentaculatBoss::onUpdate(float frametime)
{
    m_animator.updateSubRect(*this, frametime);

    m_timer += frametime;

    sf::Vector2f targetPos = m_target->getCenter();
    m_weapon.shoot(targetPos);

    sf::Vector2f pos = getPosition();
    switch (m_state) {
        case LURK: {
            float maxY = EntityManager::getInstance().getSize().y - getHeight() - 40;
            if ((int) pos.y < 40 || (int) pos.y > maxY) {
                setY(math::clamp<float>(pos.y, 40, maxY));
                m_speed.y *= -1;
            }
            if ((int) m_timer > 10) {
                m_timer = 0.f;
                m_state = IDLE;
                m_speed = {0.f, 0.f};
            }
            break;
        }
        case IDLE:
            if ((int) m_timer > 2) {
                m_timer = 0.f;
                m_state = CHARGE;
                float angle = math::angle(pos, targetPos);
                m_speed.x = std::cos(angle) * 350;
                m_speed.y = -std::sin(angle) * 350;
            }
            break;
        case CHARGE: {
            sf::FloatRect zone = EntityManager::getInstance().getViewZone();
            // Shrink the view zone to delimit allowed area for movements
            zone.left += 10;
            zone.top += 20;
            zone.width -= (getWidth() + 60);
            zone.height -= (getHeight() + 50);
            if (pos.x < zone.left || pos.y < zone.top || pos.y > zone.top + zone.height) {
                if (pos.x < zone.left) {
                    setX(zone.left);
                }
                setY(math::clamp<float>(pos.y, zone.top, zone.top + zone.height));

                // Go back to initial position
                m_speed.x *= -1;
                m_speed.y *= -1;
            } else if (pos.x > zone.left + zone.width) {
                setX(zone.left + zone.width);
                m_state = LURK;
                m_speed = {0.f, 120.f};
            }
            break;
        }
    }
    move(m_speed.x * frametime, m_speed.y * frametime);
    updateDamageFlash(frametime);
}


void TentaculatBoss::onDestroy()
{
    EntityManager::getInstance().createGreenParticles(getCenter(), 150);
    // Low-pitched explosion
    Services::getSoundSystem().playSound("boom.ogg", 0.3f);
}
