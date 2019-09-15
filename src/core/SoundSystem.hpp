#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <SFML/Audio.hpp>
#include "vendor/sfMod/sfMod.hpp"

/**
 * Class for playing sound effects and music
 */
class SoundSystem {
public:
    SoundSystem();

    /**
     * Control music
     */
    bool openMusicFromFile(const std::string& path);
    void playMusic();
    void stopMusic();
    void pauseMusic();

    /**
     * Play a sound effect
     * @param name: sound buffer filename in the Resources class loader
     */
    void playSound(const std::string& name, float pitch = 1.f);
    void playSound(const sf::SoundBuffer& soundbuffer, float pitch = 1.f);

    /**
     * Control music volume
     */
    void setMusicVolume(int volume);
    int getMusicVolume() const;

    /**
     * Control sound effects volume
     */
    void setSoundVolume(int volume);
    int getSoundVolume() const;

    /**
     * Turn music on/off
     */
    void enableMusic(bool enabled);
    bool isMusicEnabled() const;

    /**
     * Turn sound effects on/off
     */
    void enableSound(bool enabled);
    bool isSoundEnabled() const;

    /**
     * Stop music and sound effects
     */
    void atExit();

private:
    static const int MAX_SOUNDS = 20;

    sf::Sound  m_sounds[MAX_SOUNDS];
    int        m_lastSoundPlayed;
    sfmod::Mod m_music;
    int        m_musicVolume;
    int        m_soundVolume;
    bool       m_enableMusic;
    bool       m_enableSound;
};

#endif
