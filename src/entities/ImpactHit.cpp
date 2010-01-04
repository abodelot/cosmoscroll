#include "ImpactHit.hpp"
#include "EntityManager.hpp"
#include "../core/ParticleSystem.hpp"

#include "../utils/Math.hpp"

#define IMPACT_RADIUS 256

class Impact
{
public:
	Impact(Hit& hit) :
		hit_(hit)
	{
		radius_ = IMPACT_RADIUS;
	}
	void operator()(Entity& e)
	{
		if (math::distance(e.GetPosition(), hit_.GetPosition()) < radius_)
		{
			e.TakeDamage(hit_.GetCollideDamage());
			if (e.IsDead())
				ParticleSystem::GetInstance().AddImpact(e.GetPosition(), 10);
		}
	}

private:
	Hit& hit_;
	int radius_;
};

#define PARTICLES_PER_HIT 64

ImpactHit::ImpactHit(Entity::Team team, const sf::Vector2f& position, float angle,
		const sf::Image* image, int speed, int damage):
	Hit(team, position, angle, image, speed, damage)
{
	ParticleSystem::GetInstance().AddSmoke(PARTICLES_PER_HIT, this);
}


ImpactHit::~ImpactHit()
{
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void ImpactHit::OnCollide(Entity& entity)
{
	if (!IsDead())
	{
		Hit::OnCollide(entity);
		if (IsDead())
		{
			ParticleSystem::GetInstance().AddImpact(GetPosition(), 300);
			Impact impact(*this);
			EntityManager::GetInstance().ApplyToEach(impact);
		}
	}
}

