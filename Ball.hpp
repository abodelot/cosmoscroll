#ifndef BALL_HPP
#define BALL_HPP

#include "Ennemy.hpp"

/*
 * Une balle rebondissante
 */
class Ball: public Entity
{
public:
    Ball();
    
    void Move(float frametime);
    
    void Collide(Entity& ent);
private:

	void Spawn();
	sf::Vector2f movement_;

	static const sf::Vector2f ball_offset_;
};

#endif /* guard BALL_HPP */

