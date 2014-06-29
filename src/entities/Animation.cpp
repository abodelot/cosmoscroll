#include "Animation.hpp"


Animation::Animation():
	m_texture(NULL),
	m_delay(1.f)
{
}


void Animation::setDelay(float delay)
{
	m_delay = delay;
}


float Animation::getDelay() const
{
	return m_delay;
}


void Animation::setTexture(const sf::Texture& texture)
{
	m_texture = &texture;
}


const sf::Texture& Animation::getTexture() const
{
	return *m_texture;
}


float Animation::getDuration() const
{
	return m_frames.size() * m_delay;
}


void Animation::addFrame(const sf::IntRect& subrect)
{
	m_frames.push_back(subrect);
}


const sf::IntRect& Animation::getFrame(int index) const
{
	return m_frames[index];
}


size_t Animation::getFrameCount() const
{
	return m_frames.size();
}

