#include "Animation.hpp"


Animation::Animation()
{
	image_ = NULL;
	delay_ = 1.0f;
}


float Animation::getDuration() const
{
	return subrects_.size() * delay_;
}


void Animation::addFrame(const sf::IntRect& subrect)
{
	subrects_.push_back(subrect);
}


void Animation::addFrame(int left, int top, int width, int height)
{
	addFrame(sf::IntRect(left, top, width, height));
}


void Animation::setDelay(float delay)
{
	delay_ = delay;
}


void Animation::setTexture(const sf::Texture& image)
{
	image_ = &image;
}


const sf::Texture& Animation::getTexture() const
{
	return *image_;
}
