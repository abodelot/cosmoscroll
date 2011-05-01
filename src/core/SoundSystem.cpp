#include <iostream>

#include "SoundSystem.hpp"
#include "utils/MediaManager.hpp"

#define DEFAULT_MUSIC  "space"
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



void SoundSystem::PlayMusic(const std::string& music_name)
{
	SetMusic(music_name);

	if (enable_music_)
	{
		music_->Play();
	}
}


const std::string& SoundSystem::GetMusicName() const
{
	return music_name_;
}


void SoundSystem::SetMusic(const std::string& music_name)
{
	sf::SoundStream* music = NULL;
	try
	{
		music = GET_DUMB_MUSIC(music_name.c_str());
		music_name_ = music_name;
	}
	catch (MediaNotFoundException& e)
	{
		std::cerr << "[sound] music '" << music_name << "' not found"  << std::endl;
		music = GET_DUMB_MUSIC(DEFAULT_MUSIC);
		music_name_ = DEFAULT_MUSIC;
	}

	if (music != music_)
	{
		StopMusic();
		music_ = music;
		music_->SetVolume(music_volume_);
	}
}


void SoundSystem::StopMusic()
{
	if (music_ != NULL && music_->GetStatus() == sf::Sound::Playing)
	{
		music_->Stop();
	}
}


void SoundSystem::PlaySound(const char* sound_name)
{
	if (enable_sound_)
	{
		static MediaManager& media = MediaManager::GetInstance();
		if (last_used_ == MAX_SOUNDS)
		{
			last_used_ = 0;
		}
		sf::Sound& sound = sounds_[last_used_];
		if (sound.GetStatus() == sf::Sound::Playing)
		{
			sound.Stop();
		}
		sound.SetBuffer(media.GetSoundBuf(sound_name));
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
