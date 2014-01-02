#include "Damageable.hpp"
#include "Projectile.hpp"
#include "Explosion.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"

#define FLASH_DELAY 0.3f


Damageable::Damageable():
	m_hp(1),
	m_flash_timer(0)
{
}


void Damageable::collides(Entity& entity)
{
	entity.onCollision(*this);
}


void Damageable::onCollision(Damageable& entity)
{
	if (getTeam() != entity.getTeam() && m_hp > 0 && entity.m_hp > 0)
	{
		takeDamage(1);
		entity.takeDamage(1);
	}
}


void Damageable::onCollision(Projectile& projectile)
{
	// Ignore friendly fire
	if (getTeam() != projectile.getTeam() && m_hp > 0)
	{
		takeDamage(projectile.getDamage());
		// Destroy projectile
		ParticleSystem::GetInstance().ImpactSfx(projectile.getPosition(), 10);
		projectile.kill();
	}
}


void Damageable::onDestroy()
{
	Explosion* explosion = new Explosion;
	explosion->setPosition(getCenter());
	EntityManager::getInstance().addEntity(explosion);
}


void Damageable::takeDamage(int damage)
{
	if (m_hp <= 0) // Prevent from calling onDestroy twice by mistake
		return;

	m_hp -= damage;
	if (m_hp <= 0)
	{
		kill();
	}
	else
	{
		initDamageFlash();
	}
}


int Damageable::getHP() const
{
	return m_hp;
}


void Damageable::setHP(int hp)
{
	m_hp = hp;
}


int Damageable::updateHP(int diff)
{
	return m_hp += diff;
}


void Damageable::initDamageFlash()
{
	if (m_flash_timer <= 0)
		m_flash_timer = FLASH_DELAY;
}


void Damageable::updateDamageFlash(float frametime)
{
	if (m_flash_timer >= 0)
	{
		m_flash_timer -= frametime;
		int value = 255 - (255 * m_flash_timer / FLASH_DELAY);
		setColor(sf::Color(255, value, value));
		if (m_flash_timer <= 0)
		{
			setColor(sf::Color::White);
		}
	}
}
