#include "Animation.hpp"


Animation::Animation():
	m_texture(NULL),
	m_delay(1.f)
{
}


float Animation::getDuration() const
{
	return m_frames.size() * m_delay;
}


void Animation::addFrame(const sf::IntRect& subrect)
{
	m_frames.push_back(subrect);
}


const sf::IntRect& Animation::getFrame(int num_frame) const
{
	return m_frames[num_frame];
}


void Animation::setDelay(float delay)
{
	m_delay = delay;
}


void Animation::setTexture(const sf::Texture& image)
{
	m_texture = &image;
}


const sf::Texture& Animation::getTexture() const
{
	return *m_texture;
}
