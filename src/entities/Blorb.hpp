#ifndef BLORB_HPP
#define BLORB_HPP

#include "Ennemy.hpp"
#include "../core/Animated.hpp"

/*
 * Un ennemy historique :)
 */
class Blorb: public Ennemy, public Animated
{
public:
	Blorb(const sf::Vector2f& offset, Entity* target);
	
	void Move(float frametime);
};

#endif /* guard BLORB_HPP */

