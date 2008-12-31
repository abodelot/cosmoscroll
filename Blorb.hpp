#ifndef BLORB_HPP
#define BLORB_HPP

#include "Ennemy.hpp"
#include "Animation.hpp"

/*
 * Un ennemy historique :)
 */
class Blorb: public Ennemy
{
public:
	Blorb(const sf::Vector2f& offset, Entity* target);
	
	void Move(float frametime);
	
private:
	float timer_;
	int frame_;
	const Animation& animation_;
};

#endif /* guard BLORB_HPP */

