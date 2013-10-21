#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Projectile.hpp"

class Missile: public Projectile
{
public:
	Missile(Entity* emitter, float angle, const sf::Texture& image, int speed, int damage);

	~Missile();

	// callbacks ---------------------------------------------------------------

	void onDestroy();

private:
	float m_angle;
};

#endif // MISSILE_HPP
