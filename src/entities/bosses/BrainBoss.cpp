#include "BrainBoss.hpp"
#include "entities/EntityManager.hpp"
#include "core/Services.hpp"
#include "core/Resources.hpp"

#define ID_BRAIN 1
#define ID_EYE   2
#define SPEED    40.f

BrainBoss::BrainBoss():
    m_state(MOVE),
    m_stateTimer(0),
    m_weapon_angle(0),
    m_xspeed(-SPEED)
{
    setTeam(Entity::BAD);

    Part brain(ID_BRAIN);
    brain.setDestructible(false);
    m_animator.setAnimation(brain, Services::getFactory().getAnimation("brain-boss"));
    addPart(brain, 0, 0);

    Part eye(ID_EYE, 150);
    m_eye_animator.setAnimation(eye, Services::getFactory().getAnimation("brain-boss-eye"));
    addPart(eye, 0, 30);

    m_weapon.init("laser-green");
    m_weapon.setFireRate(4);
    m_weapon.setOwner(this);
    m_weapon.setPosition(8, 38);
}


void BrainBoss::onUpdate(float frametime)
{
    updateParts(frametime);
    m_animator.updateSubRect(getPartAt(0), frametime);
    m_stateTimer += frametime;
    const sf::FloatRect& rect = EntityManager::getInstance().getViewZone();
    switch (m_state)
    {
        case MOVE:
        {
            // Move in a sinus wave
            sf::Vector2f pos = getPosition();
            if (pos.x < rect.left + 300) {
                m_xspeed = SPEED;
            } else if (pos.x > rect.left + 500) {
                m_xspeed = -SPEED;
            }
            pos.x += m_xspeed * frametime;
            pos.y = std::sin(pos.x * 0.05) * 100 + 200;
            setPosition(pos);

            // Shoot in half-circle, from 0.5 PI to 1.5 PI
            m_weapon_angle += math::PI * frametime;
            if (m_weapon_angle > math::PI * 1.5f) {
                m_weapon_angle = math::PI * 0.5f;
            }
            m_weapon.shoot(m_weapon_angle);

            if (m_stateTimer > 6.f) {
                m_stateTimer = 0.f;
                m_state = WAIT;
                m_eye_animator.setFrame(getPartAt(1), 4);
            } else {
                m_eye_animator.updateSubRect(getPartAt(1), frametime);
            }
            break;
        }
        case WAIT:
            if (m_stateTimer > 4.f) {
                m_stateTimer = 0.f;
                m_state = WAIT_ATTACK;
                if (getPartAt(1).getHP() < 100)
                    m_weapon.setMultiply(3);
                m_weapon.setFireRate(12);
            }
            break;
        case WAIT_ATTACK:
            m_weapon.shoot(math::PI);
            if (m_stateTimer > 3.f) {
                m_stateTimer = 0.f;
                m_state = MOVE;
                m_weapon.setMultiply(1);
                m_weapon.setFireRate(4);
            }
            break;
    }
}


void BrainBoss::onPartDamaged(const Part& part)
{
    // Flash the brain when eye is damaged
    if (part.getID() == ID_EYE) {
        getPartAt(0).initDamageFlash();
        // Keep the eye wide open
        getPartAt(1).setTextureRect(m_eye_animator.getAnimation()->getFrame(5));
    }
}


void BrainBoss::onPartDestroyed(const Part& part)
{
    if (part.getID() == ID_EYE) {
        kill();
        EntityManager::getInstance().createGreenParticles(getCenter(), 150);
        // Low-pitched explosion
        Services::getSoundSystem().playSound("boom.ogg", 0.3f);
    }
}
