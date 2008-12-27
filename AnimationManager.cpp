#include "AnimationManager.hpp"


AnimationManager& AnimationManager::GetInstance()
{
	static AnimationManager self;
	return self;
}


AnimationManager::AnimationManager()
{

	Animation& anim = animations_[CAPSULE];
	anim.delay = 0.1f;
	const int NB_ANIM = 8;
	const int SIZE = 32;
	for (int i = 0; i < NB_ANIM; ++i)
	{
		anim.subrects.push_back(sf::IntRect(i * SIZE, 0, (i + 1) * SIZE, SIZE));
	}
}


const AnimationManager::Animation& AnimationManager::GetAnimation(Animated anim) const
{
	return animations_[anim];
}

