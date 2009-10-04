#include "SoundSystem.hpp"
#include "../utils/MediaManager.hpp"


SoundSystem& SoundSystem::GetInstance()
{
	static SoundSystem self;
	return self;
}


SoundSystem::SoundSystem()
{
	last_used_ = 0;
	music_ = NULL;
	enable_music_ = true;
}


SoundSystem::~SoundSystem()
{
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		if (sounds_[i].GetStatus() == sf::Sound::Playing)
		{
			sounds_[i].Stop();
		}
	}
	if (music_ != NULL && music_->GetStatus() == sf::Sound::Playing)
	{
		music_->Stop();
	}
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


void SoundSystem::SetSoundVolume(int volume)
{
	for (int i = 0; i < MAX_SOUNDS; ++i)
	{
		sounds_[i].SetVolume(volume);
	}
}


void SoundSystem::PlayMusic(const char* music_name)
{
#if 0
	float vol = volume_;
	sf::Music* next = MediaManager::GetInstance().GetMusic(music_name);
	if (enable_music_ && next != music_)
	{
		if (music_ != NULL)
		{
			music_->Stop();
		}
		music_ = next;
		music_->Play();
	}
#endif
}


void SoundSystem::PlayMusic(const std::string& music_name)
{
	PlayMusic(music_name.c_str());
}


void SoundSystem::StopMusic()
{
	music_->Stop();
}


void SoundSystem::EnableMusic(bool enabled)
{
	enable_music_ = enabled;
}
