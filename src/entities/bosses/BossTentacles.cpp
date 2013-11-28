#include "BossTentacles.hpp"
#include "entities/EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "entities/Player.hpp"
#include "utils/Math.hpp"

#define MAX_X 400
#define MAX_Y (EntityManager::getInstance().getHeight() - getHeight())


BossTentacles::BossTentacles():
	m_target(NULL)
{
	setTeam(Entity::BAD);
	setHP(300);

	// init weapons
	m_weapon.init("laser-pink");
	m_weapon.setOwner(this);
	m_weapon.setPosition({74, 42});
	m_weapon.setMultiply(3);

	move_ = INIT;
	speed_x_ = -100;
	speed_y_ = 0;
	timer_ = 0;

	m_animator.setAnimation(*this, EntityManager::getInstance().getAnimation("boss-tentacles"));
}


void BossTentacles::onUpdate(float frametime)
{
	m_animator.updateSubRect(*this, frametime);

	timer_ += frametime;

	sf::Vector2f target_pos = m_target->getCenter();

	m_weapon.shoot(target_pos);


	sf::Vector2f pos = getPosition();
	switch (move_)
	{
		case INIT:
			if (pos.x < MAX_X)
			{
				setX(MAX_X);
				move_ = LURK;
				speed_y_ = 120;
				speed_x_ = 0;
			}
			break;
		case LURK:
			if ((int) pos.y < 40 || (int) pos.y > MAX_Y - 40)
			{
				if      ((int) pos.y < 40)         setY(40);
				else if ((int) pos.y > MAX_Y - 40) setY(MAX_Y - 40);
				speed_y_ *= -1;
			}
			if ((int) timer_ > 10)
			{
				timer_ = 0.f;
				move_ = IDLE;
				speed_x_ = speed_y_ = 0;
			}
			break;
		case IDLE:
			if ((int) timer_ > 2)
			{
				timer_ = 0.f;
				move_ = CHARGE;
				float angle = math::angle(target_pos, pos);
				speed_x_ = std::cos(angle) * 350;
				speed_y_ = -std::sin(angle) * 350;
			}
			break;
		case CHARGE:
			if ((int) pos.x < 10 || (int) pos.y < 10 || (int) pos.y > MAX_Y - 10)
			{
				if ((int) pos.x < 10)         setX(10);
				if ((int) pos.y < 10)         setY(10);
				if ((int) pos.y > MAX_Y - 10) setY(MAX_Y - 10);
				speed_x_ *= -1;
				speed_y_ *= -1;
			}
			else if ((int) pos.x > MAX_X)
			{
				setX(MAX_X);
				move_ = LURK;
				speed_x_ = 0;
				speed_y_ = 120;
			}
			break;
	}
	sf::Sprite::move(speed_x_ * frametime, speed_y_ * frametime);
	m_weapon.onUpdate(frametime);

	updateDamageFlash(frametime);
}


void BossTentacles::onDestroy()
{
	sf::Vector2f pos = getCenter();
	ParticleSystem::GetInstance().GreenImpactSfx(pos, 100);
}


void BossTentacles::onInit()
{
    m_target = EntityManager::getInstance().GetPlayerShip();
}

