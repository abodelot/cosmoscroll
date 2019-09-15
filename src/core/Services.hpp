#ifndef SERVICES_HPP
#define SERVICES_HPP

#include "utils/I18n.hpp"
#include "Collisions.hpp"
#include "Factory.hpp"
#include "MessageSystem.hpp"
#include "HUD.hpp"
#include "LevelParser.hpp"
#include "particles/ParticleSystem.hpp"
#include "SoundSystem.hpp"

/**
 * Singleton service locator
 * Encapsulate each single instance of game services
 */
class Services {
public:
    static Services& getInstance();

    static Collisions& getCollisions();
    static Factory& getFactory();
    static HUD& getHUD();
    static LevelParser& getLevelParser();
    static MessageSystem& getMessageSystem();
    static ParticleSystem& getParticleSystem();
    static SoundSystem& getSoundSystem();

private:
    Services() {}

    Collisions m_collisions;
    Factory m_factory;
    HUD m_hud;
    LevelParser m_levelParser;
    MessageSystem m_messageSystem;
    ParticleSystem m_particleSystem;
    SoundSystem m_soundSystem;
};

#endif
