#include "Blorb.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"
#include "ParticleSystem.hpp"

#define SHIP_SPEED      25 // pixels/seconde
#define GUN_OFFSET sf::Vector2f(-1, 14)


Blorb::Blorb(const sf::Vector2f& offset, Entity** target) :
    Entity(GET_IMG("WIP_capsule"), offset),
    weapon_(Weapon::PLASMACANNON)
{
    old_orient_ = false;
    target_ = target;
}


void Blorb::Hit(int damage)
{
    Entity::Hit(damage);
    if (health_ <= 0)
    {
        ParticleSystem::GetInstance().AddExplosion(sprite_.GetPosition());   
    }
}


void Blorb::Move(float frametime)
{
    float velocity = SHIP_SPEED * frametime;
    float vy = 0;
    float vx = 0;
    sf::Vector2f player_pos = (**target_).GetPosition();
    sf::Vector2f blorb_pos = sprite_.GetPosition();
    
    bool out = false;
//    if (blorb_pos.x < 0)
//    {
//    	out = true;
//    	blorb_pos.x = 0;
//    }
//    if (blorb_pos.y < 0)
//    {
//    	out = true;
//    	blorb_pos.y = 0;
//    }
    
    if (!out)
    {
        bool right = false;
        //std::cerr << "Player: " << player_pos.x << ", " << player_pos.y << std::endl;
	    if (blorb_pos.x > player_pos.x)
	    {
	    	vx = -velocity;
	    }
	    else if (blorb_pos.x < player_pos.x)
	    {
	    	vx = velocity;
	    	right = true;
	    }
	    
	    if (blorb_pos.y > player_pos.y)
	    	vy = -velocity;
	    else if (blorb_pos.y < player_pos.y)
	    	vy = velocity;
	    
	    if (right != old_orient_)
	    	sprite_.FlipX(!right);
	    old_orient_ = right;
    }
    sprite_.Move(vx, vy);
    weapon_.Update(frametime);
}


void Blorb::Action()
{
    float my_y = sprite_.GetPosition().y;
    float player_y = (**target_).GetPosition().y;
    // Doit on tirer ?
    if (std::abs(player_y - my_y) < 30)
    {
        sf::Vector2f offset = GUN_OFFSET; 
        float radians = PI;
        if (!old_orient_)
        {
            // le premier pixel à la gauche de l'image
            offset.x = -1;
            radians *= -1;
        }
        weapon_.Shoot(sprite_.GetPosition() + offset, radians);
    }
}

