#include <iostream>

#include "Ball.hpp"
#include "Game.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"


#define SPEED 75

/*

	Gameplay Balle:
	
		Sur collision bord haut bord bas = Rebond.
		Sur collision playership = Rebond.
		Sur collision bord gauche bord droit -> Player du coté, -1 point: respawn.

*/
	const sf::Vector2f Ball::ball_offset_ (WIN_HEIGHT / 2,  WIN_WIDTH / 2 );


	
Ball::Ball() :
    Entity(GET_IMG("tennis_ball"), ball_offset_, 42)
{
	sf::Randomizer::SetSeed((int) time(NULL));
	use_limits_ = true;
	Spawn();
}

void Ball::Spawn()
{
	static sf::Vector2f corrected_;
	corrected_ = ball_offset_;
	corrected_.x += sprite_.GetSize().x /2 ;
	corrected_.y -= sprite_.GetSize().y;
	sprite_.SetPosition(corrected_);
	movement_.x = (15 + sf::Randomizer::Random(-24, 42))*3;
	movement_.y = (15 + sf::Randomizer::Random(-24, 42))*3;
}


void Ball::Move(float frametime)
{
	static const float WIDTH = sprite_.GetSize().x;
	static const float HEIGHT = sprite_.GetSize().y;
	static Game& g_ = Game::GetInstance();
	sf::Vector2f pos = sprite_.GetPosition();
	pos.y += movement_.y * frametime;

	// On rebondit sur les bords haut et bas
	if (pos.y > y_limits_.y - HEIGHT || pos.y < y_limits_.x)
	{
		movement_.y = -movement_.y;
		pos = sprite_.GetPosition();
	}
	pos.x += movement_.x * frametime;

	sprite_.SetPosition(pos.x, pos.y);

    if (pos.x < x_limits_.x)
    {
    	std::cerr << "Sorti a gauche\n";
    	g_.Hit(1);
    	Spawn();
    }
    else if (pos.x > x_limits_.y - WIDTH)
    {
    	std::cerr << "Sorti a droite\n";
    	g_.Hit(2);
    	Spawn();
    }
}

void Ball::Collide(Entity& ent)
{
	sf::Vector2f ship_pos = ent.GetPosition();
}



