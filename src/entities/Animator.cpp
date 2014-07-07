#include "Animator.hpp"


Animator::Animator():
	m_animation(NULL),
	m_frame(0),
	m_timer(0.f)
{
}


void Animator::setAnimation(sf::Sprite& sprite, const Animation& animation)
{
	m_animation = &animation;
	reset(sprite);
}


const Animation* Animator::getAnimation() const
{
	return m_animation;
}


void Animator::reset(sf::Sprite& sprite)
{
	if (m_animation != NULL)
	{
		sprite.setTexture(m_animation->getTexture());
		setFrame(sprite, 0);
	}
}


void Animator::updateSubRect(sf::Sprite& sprite, float frametime)
{
	m_timer -= frametime;
	if (m_timer <= 0)
	{
		m_timer = m_animation->getDelay();
		m_frame = (m_frame + 1) % m_animation->getFrameCount(); // Next frame
		sprite.setTextureRect(m_animation->getFrame(m_frame));
	}
}


void Animator::setFrame(sf::Sprite& sprite, size_t index)
{
	if (index < m_animation->getFrameCount())
	{
		m_frame = index;
		m_timer = m_animation->getDelay();
		sprite.setTextureRect(m_animation->getFrame(index));
	}
}

