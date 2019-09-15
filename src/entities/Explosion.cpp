#include "Explosion.hpp"
#include "core/Services.hpp"
#include "core/SoundSystem.hpp"


Explosion::Explosion()
{
    m_animator.setAnimation(*this, Services::getFactory().getAnimation("explosion"));
    Services::getSoundSystem().playSound("boom.ogg");

    setOrigin(getWidth() / 2, getHeight() / 2);
}


void Explosion::collides(Entity& entity)
{
    entity.onCollision(*this);
}


void Explosion::onUpdate(float frametime)
{
    m_animator.updateSubRect(*this, frametime);
    if (m_clock.getElapsedTime().asSeconds() > m_animator.getAnimation()->getDuration()) {
        kill();
    }
}
