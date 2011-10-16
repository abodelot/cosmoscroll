#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Entity.hpp"

#include <SFML/Graphics.hpp>

/**
 * Un astéroïde pouvant se diviser en petits morceaux
 */
class Asteroid: public Entity
{
public:
	enum Size
	{
		SMALL, MEDIUM, BIG
	};

	/**
	 * Création d'un astéroïde
	 * @param offset: position
	 * @param size: taille
	 * @param angle: orientation de la trajectoire (degrés)
	 */
	Asteroid(const sf::Vector2f& offset, Size size, float angle=180);

	// override
	Asteroid* Clone() const;

	// override
	void Update(float frametime);

	// override
	void OnDestroy();

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

