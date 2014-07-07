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

	void setAnimation(sf::Sprite& sprite, const Animation& animation);

	const Animation* getAnimation() const;

	void reset(sf::Sprite& sprite);

	/**
	 * Update texture subrect on a animated sprite
	 * @param sprite: sprite to update
	 */
	void updateSubRect(sf::Sprite& sprite, float frametime);

	void setFrame(sf::Sprite&, size_t index);

private:
	const Animation* m_animation;
	size_t           m_frame; // current frame index
	float            m_timer;
};

#endif // ANIMATOR_HPP

