#ifndef IMPACTHIT_HPP
#define IMPACTHIT_HPP

#include "Hit.hpp"

/**
 * Bullet entity with damage zone
 */
class ImpactHit: public Hit
{
public:
	ImpactHit(Entity::Team team, const sf::Vector2f& offset, float angle,
		const sf::Image* image, int speed, int damage);

	~ImpactHit();

	// override
	void OnCollide(Entity& entity);
};

#endif // IMPACTHIT_HPP
