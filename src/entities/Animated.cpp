#include "Animated.hpp"


Animated::Animated(const Animation& animation)
{
	SetAnimation(animation);
}


Animated::Animated()
{
	animation_ = NULL;
	timer_ = 0.f;
	frame_ = 0;
}


void Animated::Reset(sf::Sprite& sprite)
{
	if (animation_ != NULL)
	{
		sprite.setTexture(animation_->getTexture());
		sprite.setTextureRect(animation_->GetFrame(0));
	}
}


void Animated::UpdateSubRect(sf::Sprite& sprite, float frametime)
{
	timer_ -= frametime;
	if (timer_ <= 0)
	{
		timer_ = animation_->GetDelay();
		frame_ = (frame_ + 1) % animation_->GetSize();
		sprite.setTextureRect(animation_->GetFrame(frame_));
	}
}


void Animated::SetAnimation(const Animation& animation)
{
	animation_ = &animation;
	timer_ = animation.GetDelay();
	frame_ = 0;
}


const Animation* Animated::GetAnimation() const
{
	return animation_;
}
