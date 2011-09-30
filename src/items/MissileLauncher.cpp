#include "MissileLauncher.hpp"
#include "entities/EntityManager.hpp"
#include "entities/ImpactHit.hpp"



void MissileLauncher::ThrowHit(const sf::Vector2f& offset, float angle)
{
	EntityManager::GetInstance().AddEntity(
		new ImpactHit(GetOwner()->GetTeam(),
		offset, angle, GetImage(), GetVelocity(), GetDamage())
	);
}


