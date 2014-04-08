#include "FlyingSaucerBoss.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "entities/Spaceship.hpp"
#include "core/ParticleSystem.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

#define CIRCLE_RADIUS 50
#define CIRCLE_CENTER_X 400
#define CIRCLE_CENTER_Y 150
#define CIRCLE_ROTATION_SPEED (math::PI * 0.8)


FlyingSaucerBoss::FlyingSaucerBoss():
	m_timer(0),
	m_angle(0)
{
	setHP(400);
	setTexture(Resources::getTexture("entities/flying-saucer-boss.png"));
	setY(CIRCLE_CENTER_Y);

	m_left_tube.init("laser-pink");
	m_left_tube.setOwner(this);
	m_left_tube.setPosition({34, 122});
	m_left_tube.setMultiply(2);
	m_right_tube.init("laser-pink");
	m_right_tube.setOwner(this);
	m_right_tube.setPosition({157, 122});
	m_right_tube.setMultiply(2);
	setTeam(Entity::BAD);
	m_target = NULL;
}


void FlyingSaucerBoss::onUpdate(float frametime)
{
    //Attack
	sf::Vector2f target_pos = m_target->getCenter();

	m_left_tube.shoot(target_pos);
	m_right_tube.shoot(target_pos);

	if (getX() > CIRCLE_CENTER_X + CIRCLE_RADIUS)
	{
				move(-100 * frametime, 0);
			}
			else
			{
				sf::Vector2f pos(CIRCLE_CENTER_X, CIRCLE_CENTER_Y);
				m_angle += CIRCLE_ROTATION_SPEED * frametime;
				pos.x += CIRCLE_RADIUS * std::cos(m_angle);
				pos.y -= CIRCLE_RADIUS * std::sin(m_angle);
				setPosition(pos);
			}
	updateDamageFlash(frametime);

	m_timer += frametime;
	if (m_timer > 5.f)
	{
		m_timer = 0.f;
		EntityManager& em = EntityManager::getInstance();
		Spaceship* mob = em.createSpaceship("s1");
		mob->setPosition(getCenter());
		em.addEntity(mob);
	}
}


void FlyingSaucerBoss::takeDamage(int damage)
{
	Damageable::takeDamage(damage);

}


void FlyingSaucerBoss::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();
}
