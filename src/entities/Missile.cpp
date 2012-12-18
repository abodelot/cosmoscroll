#include "Missile.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"

#include "utils/Math.hpp"

#define IMPACT_RADIUS 256

class Impact
{
public:
	Impact(Missile& missile) :
		m_missile(missile),
		m_radius(IMPACT_RADIUS)
	{
	}

	void operator()(Entity& e)
	{
		if (e.GetCollideFlag() & C_IGNORE_HITS)
		{
			return;
		}

		if (e.GetTeam() != m_missile.GetTeam()
			&& math::distance(e.getPosition(), m_missile.getPosition()) < m_radius)
		{
			e.TakeDamage(m_missile.GetCollideDamage());
			if (e.IsDead())
				ParticleSystem::GetInstance().ImpactSfx(e.getPosition(), 10);
		}
	}

private:
	Missile& m_missile;
	int      m_radius;
};

#define PARTICLES_PER_HIT 128

Missile::Missile(Entity* emitter, const sf::Vector2f& position, float angle,
		const sf::Texture* image, int speed, int damage):
	Projectile(emitter, position, angle, image, speed, damage)
{
	ParticleSystem::GetInstance().AddSmoke(PARTICLES_PER_HIT, this);
}


Missile::~Missile()
{
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void Missile::OnCollide(Entity& entity)
{

	Projectile::OnCollide(entity);
	if (IsDead())
	{
		ParticleSystem::GetInstance().ImpactSfx(getPosition(), 300);
		Impact impact(*this);
		EntityManager::GetInstance().ApplyToEach(impact);
	}
}

