#include "Blorb.hpp"
#include "MediaManager.hpp"


#define SPEED 75


Blorb::Blorb(const sf::Vector2f& offset, Entity* target) :
    Ennemy(offset, GET_IMG("capsule"), 3, target)
{
	frame_ = 0;
	timer_ = 0;
	anim_ = &AnimationManager::GetInstance().GetAnimation(AnimationManager::CAPSULE);
}


void Blorb::Move(float frametime)
{
	timer_ += frametime;
	if (timer_ >= anim_->delay)
	{
		frame_ = (frame_ + 1) % anim_->subrects.size();
		SetSubRect(anim_->subrects[frame_]);
		timer_ = 0;
	}
    sf::Sprite::Move(-SPEED * frametime, 0);
    KillIfOut();
}




