#include "ImpactHit.hpp"
#include "EntityManager.hpp"
#include "../core/ParticleSystem.hpp"

#include "../utils/Math.hpp"

class Impact
	{
	public:
		Impact(Hit& hit) :
			hit_(hit)
		{
			radius_ = 256;
		}
		void operator()(Entity& e)
		{
			if (math::distance(e.GetPosition(), hit_.GetPosition()) < radius_)
			{
				e.TakeDamage(hit_.GetCollideDamage());
			}
		}

	private:
		Hit& hit_;
		int radius_;
};



ImpactHit::ImpactHit(Entity::Team team, const sf::Vector2f& position, float angle,
		const sf::Image* image, int speed, int damage):
	Hit(team, position, angle, image, speed, damage)
{
	ParticleSystem& p = ParticleSystem::GetInstance();
	p.AddFollow(64, this);
}


ImpactHit::~ImpactHit()
{
	ParticleSystem::GetInstance().ClearFollow(this);
}


void ImpactHit::OnCollide(Entity& entity)
{
	if (!IsDead())
	{
		Hit::OnCollide(entity);
		if (IsDead())
		{
			ParticleSystem::GetInstance().AddImpact(GetPosition(), 512);
			Impact impact(*this);
			EntityManager::GetInstance().ApplyToEach(impact);
		}
	}
}

