#include "EvilBoss.hpp"
#include "entities/Player.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "core/Services.hpp"

#define MAX_X  360.f
#define MIN_Y  60.f
#define MAX_Y  (EntityManager::getInstance().getSize().y - getHeight() - 60.f)


EvilBoss::EvilBoss():
    m_state(EVIL),
    m_next_state(MORE_EVIL),
    m_speed(-100.f, 70.f),
    m_target(nullptr)
{
    setTexture(Resources::getTexture("entities/evil-boss.png"));
    setTextureRect(sf::IntRect(0, 0, 240, 160));
    setTeam(Entity::BAD);
    setHP(EVIL);

    // Init weapons
    m_eye_left.init("fireball");
    m_eye_left.setOwner(this);
    m_eye_left.setPosition(105, 55);
    m_eye_right.init("fireball");
    m_eye_right.setOwner(this);
    m_eye_right.setPosition(190, 55);
    // hack: disable sound on the second eye so it won't be played twice
    m_eye_right.setSound(nullptr);
}

void EvilBoss::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();
}

void EvilBoss::onUpdate(float frametime)
{
    sf::Vector2f target_pos = m_target->getCenter();

    m_eye_left.shoot(target_pos);
    m_eye_right.shoot(target_pos);

    const sf::Vector2f& pos = getPosition();
    if ((int) pos.y < MIN_Y || (int) pos.y > MAX_Y) {
        if (pos.y < MIN_Y) {
            setY(MIN_Y);
        } else if (pos.y > MAX_Y) {
            setY(MAX_Y);
        }
        m_speed.y *= -1;
    }
    move(0, m_speed.y * frametime);

    if (pos.x > MAX_X) {
        move(m_speed.x * frametime, 0);
    }

    updateDamageFlash(frametime);
}


void EvilBoss::takeDamage(int damage)
{
    Damageable::takeDamage(damage);
    if (getHP() < m_next_state && getHP() > 0 && m_state != m_next_state) {
        m_state = m_next_state;
        switch (m_state) {
            case MORE_EVIL:
                setTextureRect(sf::IntRect(240, 0, 240, 160));
                m_eye_left.setMultiply(2);
                m_eye_right.setMultiply(2);
                m_next_state = DAMN_EVIL;
                break;
            case DAMN_EVIL:
                setTextureRect(sf::IntRect(240 * 2, 0, 240, 160));
                m_eye_left.setMultiply(3);
                m_eye_right.setMultiply(3);
                break;
            default:
                break;
        }
    }
}

void EvilBoss::onDestroy()
{
    EntityManager::getInstance().createGreenParticles(getCenter(), 300);
    // Low-pitched explosion
    Services::getSoundSystem().playSound("boom.ogg", 0.2f);
}
