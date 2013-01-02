#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Entity.hpp"

/**
 * Asteroid object, can be split on smaller asteroids
 */
class Asteroid: public Entity
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
	Asteroid(const sf::Vector2f& pos, Size size, float angle=180);

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
	void SetRandomImage();

	Size size_;
	sf::Vector2f speed_;
	int rotation_speed_;
};

#endif // ASTEROID_HPP

