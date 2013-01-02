#include "Missile.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "Player.hpp"

#include "utils/Math.hpp"
#include "core/Resources.hpp"

Missile::Missile(Entity* emitter, const sf::Vector2f& position, float angle,
		const sf::Texture& image, int speed, int damage):
	Projectile(emitter, position, angle, image, speed, damage),
	m_angle(angle)
{
	ParticleSystem::GetInstance().AddSmoke(128, this);
}


Missile::~Missile()
{
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void Missile::onCollision(const Entity& entity)
{
	Projectile::onCollision(entity);
	if (isDead())
	{
		ParticleSystem::GetInstance().ImpactSfx(getPosition(), 100);
		for (int i = 0; i < 16; ++i)
		{
			float angle = math::random(m_angle - math::PI / 2, m_angle + math::PI / 2);
			int speed = math::random(300, 500);
			Projectile* p = new Projectile(EntityManager::getInstance().GetPlayerShip(), entity.getPosition(), angle,
				Resources::getTexture("ammo/laser-red.png"), speed, 3);
			EntityManager::getInstance().addEntity(p);
		}
	}
}

