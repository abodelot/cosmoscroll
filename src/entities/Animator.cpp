#include "Animator.hpp"
#include "core/Collisions.hpp"


Animator::Animator()
{
	animation_ = NULL;
	timer_ = 0.f;
	frame_ = 0;
}


void Animator::reset(sf::Sprite& sprite)
{
	if (animation_ != NULL)
	{
		sprite.setTexture(animation_->getTexture());
		sprite.setTextureRect(animation_->getFrame(0));
		timer_ = animation_->getDelay();
		frame_ = 0;
	}
}


void Animator::updateSubRect(sf::Sprite& sprite, float frametime)
{
	timer_ -= frametime;
	if (timer_ <= 0)
	{
		timer_ = animation_->getDelay();
		frame_ = (frame_ + 1) % animation_->getSize();
		sprite.setTextureRect(animation_->getFrame(frame_));
	}
}


void Animator::setAnimation(sf::Sprite& sprite, const Animation& animation)
{
	animation_ = &animation;
	reset(sprite);
	Collisions::registerTexture(&animation.getTexture());
}


const Animation* Animator::getAnimation() const
{
	return animation_;
}
