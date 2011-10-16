#ifndef ANIMATED_HPP
#define ANIMATED_HPP

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

/**
 * Base class for animated class
 */
class Animated
{
public:
	Animated(const Animation& animation);
	Animated();

	void Reset(sf::Sprite& sprite);



	/**
	 * Mettre à jour le subrect d'un sprite animé
	 * @param sprite: sprite à mettre à jour
	 * @param framtime: temps de la frame courante
	 */
	void UpdateSubRect(sf::Sprite& sprite, float frametime);


	void SetAnimation(const Animation& animation);
	const Animation* GetAnimation() const;

private:
	const Animation* animation_;
	int frame_; // indice de la frame courante
	float timer_;
};

#endif // ANIMATED_HPP

