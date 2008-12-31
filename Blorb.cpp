#include "Blorb.hpp"
#include "MediaManager.hpp"


#define SPEED 75


Blorb::Blorb(const sf::Vector2f& offset, Entity* target) :
    Ennemy(offset, GET_IMG("capsule"), 3, target),
    animation_(GET_ANIM("capsule"))
{
	frame_ = 0;
	timer_ = animation_.GetDelay();
	SetSubRect(animation_.GetFrame(0));
}


void Blorb::Move(float frametime)
{
	timer_ -= frametime;
	if (timer_ <= 0)
	{
		SetSubRect(animation_.GetFrame(frame_));
		timer_ = animation_.GetDelay();
		frame_ = (frame_ + 1) % animation_.GetSize();
	}
    sf::Sprite::Move(-SPEED * frametime, 0);
    KillIfOut();
}

