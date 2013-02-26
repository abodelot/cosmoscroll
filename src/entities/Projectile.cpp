#include "Projectile.hpp"
#include "Player.hpp"
#include "utils/Math.hpp"
#include "utils/StringUtils.hpp"
#include "core/ParticleSystem.hpp"


Projectile::Projectile(Entity* emitter, const sf::Vector2f& position, float angle,
	const sf::Texture& image, int speed, int damage) :
	Entity(position, damage)
{
	setTexture(image);
	setTeam(emitter->getTeam());
	setRotation(-math::to_deg(angle));

	// calcul du vecteur vitesse à partir de l'angle et de la vitesse
	m_speed.x = std::cos(angle) * speed + emitter->GetSpeedX();
	m_speed.y = -std::sin(angle) * speed;

	// origin is located at sprite center to allow rotation
	setOrigin(getWidth() / 2, getHeight() / 2);
}


Projectile* Projectile::clone() const
{
	return new Projectile(*this);
}


void Projectile::onUpdate(float frametime)
{
	move(m_speed.x * frametime, m_speed.y * frametime);
}


void Projectile::onCollision(const Entity& entity)
{
	// Ignore friendly entities
	if (entity.getTeam() == getTeam() || entity.toProjectile() || entity.toComplexEntity() || entity.toPowerUp())
	{
		return;
	}
	ParticleSystem::GetInstance().ImpactSfx(getPosition(), 10);
	if (getTeam() == Entity::GOOD && entity.isDead())
	{
		int points = entity.getPoints();
		if (points != 0)
		{
			std::string s = "+" + std::to_string(points);
			EntityManager& e = EntityManager::getInstance();
			e.GetPlayerShip()->UpdateScoreCounter(points);
			ParticleSystem::GetInstance().AddMessage(getPosition(), s, sf::Color(255, 128, 0));
		}
	}
	kill();
}
