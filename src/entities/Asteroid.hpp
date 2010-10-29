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

	// inherited
	Asteroid* Clone() const;

	// inherited
	void Update(float frametime);

	// inherited
	void GetCollideRect(sf::FloatRect& rect) const;

	// inherited
	void TakeDamage(int damage);

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

