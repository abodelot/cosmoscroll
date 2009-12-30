#include "SoundSystem.hpp"
#include "../utils/MediaManager.hpp"

#define DEFAULT_MUSIC  "space"


SoundSystem& SoundSystem::GetInstance()
{
	static SoundSystem self;
	return self;
}


SoundSystem::SoundSystem()
{
	last_used_ = 0;
	music_ = NULL;
	music_volume_ = 50.f;
	enable_music_ = true;
	music_name_ = DEFAULT_MUSIC;
}


SoundSystem::~SoundSystem()
{
}


void SoundSystem::PlaySound(const char* sound_name)
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


void SoundSystem::SetSoundVolume(float volume)
{
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		sounds_[i].SetVolume(volume);
	}
}


void SoundSystem::PlayMusic(const std::string& music_name, bool force_enable)
{
	if (force_enable)
	{
		enable_music_ = true;
	}
	sf::SoundStream* music = NULL;
	try
	{
		music = GET_DUMB_MUSIC(music_name.c_str());
		music_name_ = music_name;
	}
	catch (MediaNotFoundException& e)
	{
		music = GET_DUMB_MUSIC(DEFAULT_MUSIC);
		music_name_ = DEFAULT_MUSIC;
	}

	if (enable_music_ && music != music_)
	{
		StopMusic();

		music_ = music;
		music_->SetVolume(music_volume_);
		music_->Play();
	}
}


const std::string& SoundSystem::GetMusicName() const
{
	return music_name_;
}


void SoundSystem::SetMusicName(const std::string& music_name)
{
	music_name_ = music_name;
}


void SoundSystem::StopMusic()
{
	if (music_ != NULL && music_->GetStatus() == sf::Sound::Playing)
	{
		music_->Stop();
	}
}


void SoundSystem::SetMusicVolume(float volume)
{
	music_volume_ = volume;
	if (music_ != NULL)
	{
		music_->SetVolume(volume);
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


bool SoundSystem::IsMusicEnabled() const
{
	return enable_music_;
}


void SoundSystem::StopAll()
{
	/*for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		if (sounds_[i].GetStatus() == sf::Sound::Playing)
		{
			sounds_[i].Stop();
		}
	}*/
	StopMusic();
}
