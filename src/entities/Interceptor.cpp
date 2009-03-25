#include "Interceptor.hpp"
#include "EntityManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"

#define SHIP_SPEED      25 // pixels/seconde
#define GUN_OFFSET        sf::Vector2f(0,  32)
#define GUN_OFFSET_INVERT sf::Vector2f(48, 30)


Interceptor::Interceptor(const sf::Vector2f& offset, Entity* target) :
    Ennemy(offset, GET_IMG("ennemy-A"), 8, target),
    weapon_(EntityManager::GetInstance().BuildWeapon(0))
{
	weapon_.SetOwner(this);
}


void Interceptor::Move(float frametime)
{
    float velocity = SHIP_SPEED * frametime;
    float vy = 0;
    float vx = 0;
    sf::Vector2f player_pos = target_->GetPosition();
    sf::Vector2f my_pos = GetPosition();

    bool flipped = false;
    if (my_pos.x > player_pos.x)
    {
    	vx = -velocity;
    }
    else if (my_pos.x < player_pos.x)
    {
    	vx = velocity;
    	flipped = true;
    }

	if (my_pos.y > player_pos.y)
	    vy = -velocity;
	else if (my_pos.y < player_pos.y)
	    vy = velocity;

	if (flipped != flipped_)
	{
	    Flip(flipped);
	}

    sf::Sprite::Move(vx, vy);
    weapon_.Update(frametime);
}


void Interceptor::Action()
{
    float radians = ANGLE(target_->GetPosition(), GetPosition());
    if (flipped_)
    {
        weapon_.Shoot(GetPosition() + GUN_OFFSET_INVERT, radians);
    }
    else
    {
        weapon_.Shoot(GetPosition() + GUN_OFFSET, radians);
    }
}

