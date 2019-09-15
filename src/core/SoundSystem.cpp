#include "SoundSystem.hpp"
#include "Resources.hpp"
#include "utils/Math.hpp"

SoundSystem::SoundSystem():
    m_lastSoundPlayed(0),
    m_music(),
    m_musicVolume(100),
    m_soundVolume(100),
    m_enableMusic(true),
    m_enableSound(true)
{
}


bool SoundSystem::openMusicFromFile(const std::string& path)
{
    if (m_music.loadFromFile(path)) {
        m_music.setLoop(true);
        m_music.setVolume(m_musicVolume);
        return true;
    }
    return false;
}


void SoundSystem::playMusic()
{
    if (m_enableMusic) {
        m_music.play();
    }
}


void SoundSystem::stopMusic()
{
    m_music.unload();
}


void SoundSystem::pauseMusic()
{
    m_music.pause();
}


void SoundSystem::playSound(const std::string& name, float pitch)
{
    playSound(Resources::getSoundBuffer(name), pitch);
}


void SoundSystem::playSound(const sf::SoundBuffer& soundbuffer, float pitch)
{
    if (m_enableSound) {
        if (m_lastSoundPlayed == MAX_SOUNDS) {
            m_lastSoundPlayed = 0;
        }
        sf::Sound& sound = m_sounds[m_lastSoundPlayed];
        if (sound.getStatus() == sf::Sound::Playing) {
            sound.stop();
        }
        sound.setBuffer(soundbuffer);
        sound.setPitch(pitch);
        sound.play();
        ++m_lastSoundPlayed;
    }
}


void SoundSystem::setMusicVolume(int volume)
{
    m_musicVolume = math::clamp(volume, 0, 100);
    m_music.setVolume(volume);
}


int SoundSystem::getMusicVolume() const
{
    return m_musicVolume;
}


void SoundSystem::setSoundVolume(int volume)
{
    m_soundVolume = math::clamp(volume, 0, 100);
    for (int i = 0; i < MAX_SOUNDS; ++i) {
        m_sounds[i].setVolume(volume);
    }
}


int SoundSystem::getSoundVolume() const
{
    return m_soundVolume;
}


void SoundSystem::enableMusic(bool enabled)
{
    m_enableMusic = enabled;
    if (!enabled) {
        stopMusic();
    }
}


void SoundSystem::enableSound(bool enabled)
{
    m_enableSound = enabled;
}


bool SoundSystem::isMusicEnabled() const
{
    return m_enableMusic;
}


bool SoundSystem::isSoundEnabled() const
{
    return m_enableSound;
}


void SoundSystem::atExit()
{
    for (int i = 0; i < MAX_SOUNDS; ++i) {
        if (m_sounds[i].getStatus() == sf::Sound::Playing) {
            m_sounds[i].stop();
        }
    }
    stopMusic();
}
