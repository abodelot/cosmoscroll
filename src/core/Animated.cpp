#include "Animated.hpp"


Animated::Animated(const Animation& animation, sf::Sprite& sprite) :
	animation_(animation)
{
	timer_ = animation_.GetDelay();
	frame_ = 0;
	
	sprite.SetSubRect(animation_.GetFrame(0));
}


void Animated::Update(float frametime, sf::Sprite& sprite)
{
	timer_ -= frametime;
	if (timer_ <= 0)
	{
		timer_ = animation_.GetDelay();
		frame_ = (frame_ + 1) % animation_.GetSize();
		sprite.SetSubRect(animation_.GetFrame(frame_));
	}
}

