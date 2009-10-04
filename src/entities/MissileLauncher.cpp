#include "MissileLauncher.hpp"
#include "EntityManager.hpp"
#include "ImpactHit.hpp"



void MissileLauncher::ThrowHit(const sf::Vector2f& offset, float angle)
{
	EntityManager::GetInstance().AddEntity(
		new ImpactHit(GetOwner()->GetTeam(),
		offset, angle, GetImage(), GetSpeed(), GetDamage())
	);
}


