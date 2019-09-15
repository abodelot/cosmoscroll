#include "Services.hpp"


Services& Services::getInstance()
{
    static Services self;
    return self;
}


Collisions& Services::getCollisions()
{
    return getInstance().m_collisions;
}


Factory& Services::getFactory()
{
    return getInstance().m_factory;
}


LevelParser& Services::getLevelParser()
{
    return getInstance().m_levelParser;
}


HUD& Services::getHUD()
{
    return getInstance().m_hud;

}


SoundSystem& Services::getSoundSystem()
{
    return getInstance().m_soundSystem;
}


ParticleSystem& Services::getParticleSystem()
{
    return getInstance().m_particleSystem;
}


MessageSystem& Services::getMessageSystem()
{
    return getInstance().m_messageSystem;
}
