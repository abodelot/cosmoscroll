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
		if (e.GetCollideFlag() & C_IGNORE_HITS)
		{
			return;
		}

		if (e.GetTeam() != hit_.GetTeam()
			&& math::distance(e.getPosition(), hit_.getPosition()) < radius_)
		{
			e.TakeDamage(hit_.GetCollideDamage());
			if (e.IsDead())
				ParticleSystem::GetInstance().ImpactSfx(e.getPosition(), 10);
		}
	}

private:
	Hit& hit_;
	int radius_;
};

#define PARTICLES_PER_HIT 128

ImpactHit::ImpactHit(Entity* emitter, const sf::Vector2f& position, float angle,
		const sf::Texture* image, int speed, int damage):
	Hit(emitter, position, angle, image, speed, damage)
{
	ParticleSystem::GetInstance().AddSmoke(PARTICLES_PER_HIT, this);
}


ImpactHit::~ImpactHit()
{
	ParticleSystem::GetInstance().ClearSmoke(this);
}


void ImpactHit::OnCollide(Entity& entity)
{

	Hit::OnCollide(entity);
	if (IsDead())
	{
		ParticleSystem::GetInstance().ImpactSfx(getPosition(), 300);
		Impact impact(*this);
		EntityManager::GetInstance().ApplyToEach(impact);
	}
}

