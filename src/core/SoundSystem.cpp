#include <iostream>

#include "SoundSystem.hpp"
#include "utils/Resources.hpp"
#include "utils/DumbMusic.hpp"
#include "utils/IniParser.hpp"

#define DEFAULT_MUSIC  "space.mod"
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


SoundSystem::SoundSystem()
{
	last_used_ = 0;
	music_ = NULL;
	music_volume_ = DEFAULT_VOLUME;
	SetSoundVolume(DEFAULT_VOLUME);
	enable_music_ = true;
	enable_sound_ = true;
	music_name_ = DEFAULT_MUSIC;
}


SoundSystem::~SoundSystem()
{
}


void SoundSystem::SetMusic(const std::string& music_name)
{
	sf::SoundStream* music = Resources::GetDumbMusic(music_name);
	music_name_ = music_name;

	if (music != music_)
	{
		StopMusic();
		music_ = music;
		music_->SetVolume(music_volume_);
	}
}


const std::string& SoundSystem::GetMusic() const
{
	return music_name_;
}


void SoundSystem::PlayMusic()
{
	if (music_ != NULL && enable_music_)
	{
		music_->Play();
	}
}


void SoundSystem::StopMusic()
{
	if (music_ != NULL)
	{
		music_->Stop();
	}
}


void SoundSystem::PauseMusic()
{
	if (music_ != NULL)
	{
		music_->Pause();
	}
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
		if (sound.GetStatus() == sf::Sound::Playing)
		{
			sound.Stop();
		}
		sound.SetBuffer(soundbuffer);
		sound.Play();
		++last_used_;
	}
}


void SoundSystem::SetMusicVolume(int volume)
{
	ensure_range(volume, 0, 100);
	music_volume_ = volume;
	if (music_ != NULL)
	{
		music_->SetVolume(volume);
	}
}


void SoundSystem::SetSoundVolume(int volume)
{
	ensure_range(volume, 0, 100);
	sound_volume_ = volume;
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		sounds_[i].SetVolume(volume);
	}
}

void SoundSystem::EnableMusic(bool enabled)
{
	enable_music_ = enabled;
	if (!enabled)
	{
		StopMusic();
	}
	else if (music_ != NULL)
	{
		music_->Play();
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
		if (sounds_[i].GetStatus() == sf::Sound::Playing)
		{
			sounds_[i].Stop();
		}
	}
	StopMusic();
}


void SoundSystem::LoadFromConfig(IniParser& config)
{
	config.SeekSection("Audio");

	if (config.Get("enable_music", enable_music_))
		EnableMusic(enable_music_);

	if (config.Get("enable_sound", enable_sound_))
		EnableSound(enable_sound_);

	if (config.Get("music_name", music_name_))
		SetMusic(music_name_);

	if (config.Get("music_volume", music_volume_))
		SetMusicVolume(music_volume_);

	if (config.Get("sound_volume", sound_volume_))
		SetSoundVolume(sound_volume_);
}


void SoundSystem::SaveToConfig(IniParser& config)
{
	config.SeekSection("Audio");
	config.Set("music_name",   music_name_);
	config.Set("enable_music", enable_music_);
	config.Set("music_volume", music_volume_);
	config.Set("enable_sound", enable_sound_);
	config.Set("sound_volume", sound_volume_);
}
