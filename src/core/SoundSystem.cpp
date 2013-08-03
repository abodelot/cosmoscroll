#include <iostream>

#include "SoundSystem.hpp"
#include "Resources.hpp"
#include "utils/IniParser.hpp"

#define DEFAULT_VOLUME 40


template <class T>
static inline void ensure_range(T& value, T min, T max)
{
	if (value > max)
	{
		value = max;
	}
	else if (value < min)
	{
		value = min;
	}
}


SoundSystem& SoundSystem::GetInstance()
{
	static SoundSystem self;
	return self;
}


SoundSystem::SoundSystem():
	last_used_(0),
	enable_music_(true),
	enable_sound_(true)
{
	SetMusicVolume(DEFAULT_VOLUME);
	SetSoundVolume(DEFAULT_VOLUME);
	DumbMusic::initDumb();
}


SoundSystem::~SoundSystem()
{
	DumbMusic::exitDumb();
}


void SoundSystem::PlayMusic(const std::string& music_name)
{
	if (enable_music_)
	{
		if (music_.getStatus() == sf::Music::Playing && music_name == music_name_)
			return;

		music_name_ = music_name;
		StopMusic();
		music_.openFromFile(Resources::getDataPath() + "/music/" + music_name);
		music_.setVolume(music_volume_);
		music_.play();
	}
}


void SoundSystem::PlayMusic()
{
	if (enable_music_)
	{
		music_.play();
	}
}


void SoundSystem::StopMusic()
{
	music_.stop();
}


void SoundSystem::PauseMusic()
{
	music_.pause();
}


void SoundSystem::PlaySound(const sf::SoundBuffer& soundbuffer)
{
	if (enable_sound_)
	{
		if (last_used_ == MAX_SOUNDS)
		{
			last_used_ = 0;
		}
		sf::Sound& sound = sounds_[last_used_];
		if (sound.getStatus() == sf::Sound::Playing)
		{
			sound.stop();
		}
		sound.setBuffer(soundbuffer);
		sound.play();
		++last_used_;
	}
}


void SoundSystem::PlaySound(const std::string& sound_name)
{
	PlaySound(Resources::getSoundBuffer(sound_name));
}


void SoundSystem::SetMusicVolume(int volume)
{
	ensure_range(volume, 0, 100);
	music_volume_ = volume;
	music_.setVolume(volume);
}


void SoundSystem::SetSoundVolume(int volume)
{
	ensure_range(volume, 0, 100);
	sound_volume_ = volume;
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		sounds_[i].setVolume(volume);
	}
}

void SoundSystem::EnableMusic(bool enabled)
{
	enable_music_ = enabled;
	if (!enabled)
	{
		StopMusic();
	}
}


void SoundSystem::EnableSound(bool enabled)
{
	enable_sound_ = enabled;
}


bool SoundSystem::IsMusicEnabled() const
{
	return enable_music_;
}


bool SoundSystem::IsSoundEnabled() const
{
	return enable_sound_;
}


void SoundSystem::StopAll()
{
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		if (sounds_[i].getStatus() == sf::Sound::Playing)
		{
			sounds_[i].stop();
		}
	}
	StopMusic();
}


void SoundSystem::LoadFromConfig(IniParser& config)
{
	config.SeekSection("Audio");

	config.Get("enable_music", enable_music_);
	config.Get("enable_sound", enable_sound_);

	if (config.Get("music_volume", music_volume_))
		SetMusicVolume(music_volume_);

	if (config.Get("sound_volume", sound_volume_))
		SetSoundVolume(sound_volume_);
}


void SoundSystem::SaveToConfig(IniParser& config)
{
	config.SeekSection("Audio");
	config.Set("enable_music", enable_music_);
	config.Set("music_volume", music_volume_);
	config.Set("enable_sound", enable_sound_);
	config.Set("sound_volume", sound_volume_);
}
