#ifndef ANIMATED_HPP
#define ANIMATED_HPP

#include <SFML/Graphics.hpp>

#include "Animation.hpp"

/**
 * Interface de sprite animé
 */
class Animated
{
public:
	Animated(const Animation& animation, sf::Sprite& sprite);

	/**
	 * Mettre à jour le subrect d'un sprite animé
	 * @param[in] framtime: temps de la frame courante
	 * @param[out] sprite: sprite à mettre à jour
	 */
	void Update(float frametime, sf::Sprite& sprite);

	const Animation& GetAnimation() const;

private:
	const Animation& animation_;
	int frame_;
	float timer_;
};

#endif // ANIMATED_HPP

