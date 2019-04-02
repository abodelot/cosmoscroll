#include "TentaculatBoss.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/Math.hpp"

#define MAX_X 400
#define MAX_Y (EntityManager::getInstance().getHeight() - getHeight())


TentaculatBoss::TentaculatBoss():
    m_state(INIT),
    m_timer(0.f),
    m_speed(-100.f, 0.f),
    m_target(NULL)
{
    setTeam(Entity::BAD);
    setHP(300);

    // init weapon
    m_weapon.init("laser-pink");
    m_weapon.setOwner(this);
    m_weapon.setPosition(74, 42);
    m_weapon.setMultiply(3);

    m_animator.setAnimation(*this, EntityManager::getInstance().getAnimation("boss-tentacles"));
}


void TentaculatBoss::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();
}


void TentaculatBoss::onUpdate(float frametime)
{
    m_animator.updateSubRect(*this, frametime);

    m_timer += frametime;

    sf::Vector2f target_pos = m_target->getCenter();
    m_weapon.shoot(target_pos);

    sf::Vector2f pos = getPosition();
    switch (m_state)
    {
        case INIT:
            if (pos.x < MAX_X)
            {
                setX(MAX_X);
                m_state = LURK;
                m_speed = {0.f, 120.f};
            }
            break;
        case LURK:
            if ((int) pos.y < 40 || (int) pos.y > MAX_Y - 40)
            {
                if      ((int) pos.y < 40)         setY(40);
                else if ((int) pos.y > MAX_Y - 40) setY(MAX_Y - 40);
                m_speed.y *= -1;
            }
            if ((int) m_timer > 10)
            {
                m_timer = 0.f;
                m_state = IDLE;
                m_speed = {0.f, 0.f};
            }
            break;
        case IDLE:
            if ((int) m_timer > 2)
            {
                m_timer = 0.f;
                m_state = CHARGE;
                float angle = math::angle(pos, target_pos);
                m_speed.x = std::cos(angle) * 350;
                m_speed.y = -std::sin(angle) * 350;
            }
            break;
        case CHARGE:
            if ((int) pos.x < 10 || (int) pos.y < 10 || (int) pos.y > MAX_Y - 10)
            {
                if ((int) pos.x < 10)         setX(10);
                if ((int) pos.y < 10)         setY(10);
                if ((int) pos.y > MAX_Y - 10) setY(MAX_Y - 10);
                // Go back to initial position
                m_speed.x *= -1;
                m_speed.y *= -1;
            }
            else if ((int) pos.x > MAX_X)
            {
                setX(MAX_X);
                m_state = LURK;
                m_speed = {0.f, 120.f};
            }
            break;
    }
    move(m_speed.x * frametime, m_speed.y * frametime);
    updateDamageFlash(frametime);
}


void TentaculatBoss::onDestroy()
{
    EntityManager::getInstance().createGreenParticles(getCenter(), 150);
    // Low-pitched explosion
    SoundSystem::playSound("boom.ogg", 0.3f);
}
