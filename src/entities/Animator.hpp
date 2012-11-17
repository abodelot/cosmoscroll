#ifndef ANIMATOR_HPP
#define ANIMATOR_HPP

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

/**
 * Utilitary class for associating a sprite to an animation
 */
class Animator
{
public:
	Animator();

	void reset(sf::Sprite& sprite);

	/**
	 * Update texture subrect on a animated sprite
	 * @param sprite: sprite to update
	 */
	void updateSubRect(sf::Sprite& sprite, float frametime);

	void setAnimation(sf::Sprite& sprite, const Animation& animation);
	const Animation* getAnimation() const;

private:
	const Animation* animation_;
	int frame_; // indice de la frame courante
	float timer_;
};

#endif // ANIMATOR_HPP

