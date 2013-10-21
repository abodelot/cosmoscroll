#include "Missile.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "Player.hpp"

#include "utils/Math.hpp"
#include "core/Resources.hpp"

Missile::Missile(Entity* emitter, float angle, const sf::Texture& image, int speed, int damage):
	Projectile(emitter, angle, image, speed, damage),
	m_angle(angle)
{
	ParticleSystem::GetInstance().AddSmoke(128, this);
}


Missile::~Missile()
{
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void Missile::onDestroy()
{
	ParticleSystem::GetInstance().ImpactSfx(getPosition(), 100);
	for (int i = 0; i < 20; ++i)
	{
		float angle = math::random(m_angle - math::PI / 2, m_angle + math::PI / 2);
		int speed = math::random(200, 600);
		Projectile* p = new Projectile(EntityManager::getInstance().GetPlayerShip(), angle,
			Resources::getTexture("ammo/laser-red.png"), speed, 10);
		p->setPosition(getPosition());
		EntityManager::getInstance().addEntity(p);
	}
}

