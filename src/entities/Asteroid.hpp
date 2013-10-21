#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Damageable.hpp"

/**
 * Asteroid object, can be split on smaller asteroids
 */
class Asteroid: public Damageable
{
public:
	enum Size
	{
		SMALL, MEDIUM, BIG
	};

	/**
	 * @param pos: initial position
	 * @param size: size type
	 * @param angle: movement direction
	 */
	Asteroid(Size size, float angle=180);

	// override
	Asteroid* clone() const;

	// override
	void onUpdate(float frametime);

	// override
	void onDestroy();

private:
	/**
	 * Set a random image, according to the size
	 */
	void setRandomImage();

	Size         m_size;
	sf::Vector2f m_speed;
	int          m_rotation_speed;
};

#endif // ASTEROID_HPP

