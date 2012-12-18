#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Projectile.hpp"

/**
 * Projectile with zone damage
 */
class Missile: public Projectile
{
public:
	Missile(Entity* emitter, const sf::Vector2f& offset, float angle,
		const sf::Texture* image, int speed, int damage);

	~Missile();

	// override
	void OnCollide(Entity& entity);
};

#endif // MISSILE_HPP
