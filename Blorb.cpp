#include "Blorb.hpp"
#include "MediaManager.hpp"


#define SPEED 75


Blorb::Blorb(const sf::Vector2f& offset, Entity* target) :
	Ennemy(offset, GET_IMG("capsule"), 3, target),
	Animated(GET_ANIM("capsule"), *this)
{
}


void Blorb::Move(float frametime)
{
	sf::Sprite::Move(-SPEED * frametime, 0);
	Animated::Update(frametime, *this);
	KillIfOut();
}

