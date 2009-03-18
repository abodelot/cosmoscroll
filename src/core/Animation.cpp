#include "Animation.hpp"


Animation::Animation()
{
	delay_ = 1.0f;
}


void Animation::AddFrame(const sf::IntRect& subrect)
{
	subrects_.push_back(subrect);
}


void Animation::AddFrame(int left, int top, int width, int height)
{
	AddFrame(sf::IntRect(left, top, left + width, top + height));
}


void Animation::SetDelay(float delay)
{
	delay_ = delay;
}

