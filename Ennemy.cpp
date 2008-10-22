#include "Ennemy.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"
#include "ParticleSystem.hpp"

#define SHIP_SPEED      25 // pixels/seconde
#define GUN_OFFSET        sf::Vector2f(0,  32)
#define GUN_OFFSET_INVERT sf::Vector2f(48, 30)


Ennemy::Ennemy(const sf::Vector2f& offset, Entity** target) :
    Entity(GET_IMG("ennemy-A"), offset),
    weapon_(Weapon::PLASMACANNON)
{
    left_ = true;
    target_ = target;
}


void Ennemy::Hit(int damage)
{
    Entity::Hit(damage);
    if (health_ <= 0)
    {
        ParticleSystem::GetInstance().AddExplosion(sprite_.GetPosition());   
    }
}


void Ennemy::Move(float frametime)
{
    float velocity = SHIP_SPEED * frametime;
    float vy = 0;
    float vx = 0;
    sf::Vector2f player_pos = (**target_).GetPosition();
    sf::Vector2f my_pos = sprite_.GetPosition();
    
    bool left = true;
    if (my_pos.x > player_pos.x)
    {
    	vx = -velocity;
    }
    else if (my_pos.x < player_pos.x)
    {
    	vx = velocity;
    	left = false;
    }
	
	if (my_pos.y > player_pos.y)
	    vy = -velocity;
	else if (my_pos.y < player_pos.y)
	    vy = velocity;
	    
	if (left != left_)
	{
	    sprite_.FlipX(!left);
	    left_ = left;
	}

    sprite_.Move(vx, vy);
    weapon_.Update(frametime);
}


void Ennemy::Action()
{
    float radians = ANGLE((**target_).GetPosition(), sprite_.GetPosition());
    if (!left_)
    {
        weapon_.Shoot(sprite_.GetPosition() + GUN_OFFSET_INVERT, radians);
    }
    else
    {
        weapon_.Shoot(sprite_.GetPosition() + GUN_OFFSET, radians);
    }
}

