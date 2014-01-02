#include "BrainBoss.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "core/ParticleSystem.hpp"

#define ID_BRAIN 1
#define ID_EYE   2
#define SPEED    40.f

BrainBoss::BrainBoss():
	m_state(MOVE),
	m_state_timer(0),
	m_weapon_angle(0),
	m_xspeed(-SPEED)
{
	setTeam(Entity::BAD);

	Part brain(ID_BRAIN);
	brain.setTexture(Resources::getTexture("entities/brain-boss.png"));
	brain.setTextureRect(sf::IntRect(0, 0, 96, 96));
	brain.setDestructible(false);
	addPart(brain, 0, 0);

	Part eye(ID_EYE, 250);
	m_eye_animator.setAnimation(eye, EntityManager::getInstance().getAnimation("brain-boss-eye"));
	addPart(eye, 0, 30);

	m_weapon.init("laser-pink");
	m_weapon.setFireRate(4);
	m_weapon.setOwner(this);
	m_weapon.setPosition(sf::Vector2f(8, 38));
}


void BrainBoss::onUpdate(float frametime)
{
	updateParts(frametime);

	Part& eye = getPartAt(1);
	m_eye_animator.updateSubRect(eye, frametime);

	m_state_timer += frametime;
	switch (m_state)
	{
		case MOVE:
		{
			// Move in a sinus wave
			sf::Vector2f pos = getPosition();
			if (pos.x < 300)
				m_xspeed = SPEED;
			else if (pos.x > 500)
				m_xspeed = -SPEED;
			pos.x += m_xspeed * frametime;
			pos.y = std::sin(pos.x * 0.05) * 100 + 200;
			setPosition(pos);

			// Shoot in half-circle, from 0.5 PI to 1.5 PI
			m_weapon_angle += math::PI * frametime;
			if (m_weapon_angle > math::PI * 1.5f)
				m_weapon_angle = math::PI * 0.5f;
			m_weapon.shoot(m_weapon_angle);

			if (m_state_timer > 6.f)
			{
				m_state_timer = 0.f;
				m_state = WAIT;
			}
			break;
		}
		case WAIT:
			if (m_state_timer > 3.f)
			{
				m_state_timer = 0.f;
				m_state = WAIT_ATTACK;
				m_weapon.setFireRate(12);
			}
			break;
		case WAIT_ATTACK:
			m_weapon.shoot(math::PI);
			if (m_state_timer > 3.f)
			{
				m_state_timer = 0.f;
				m_state = MOVE;
				m_weapon.setFireRate(4);
			}
			break;
	}
}


void BrainBoss::onPartDamaged(const Part& part)
{
	// Flash the whole entity when eye is damaged
	if (part.getID() == ID_EYE)
	{
		getPartAt(0).initDamageFlash();
	}
}


void BrainBoss::onPartDestroyed(const Part& part)
{
	if (part.getID() == ID_EYE)
	{
		kill();
		ParticleSystem::GetInstance().GreenImpactSfx(getCenter(), 100);
	}
}
