#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <string>
#include <SFML/Audio.hpp>

class SoundSystem
{
public:
	static SoundSystem& GetInstance();

	/**
	 * Jouer un son
	 */
	void PlaySound(const char* sound_name);

	void SetSoundVolume(int volume);

	/**
	 * Jouer une musique (une musique à la fois)
	 */
	void PlayMusic(const char* music_name);
	void PlayMusic(const std::string& music_name);

	/**
	 * Stopper la lecture de la musique
	 */
	void StopMusic();

	/**
	 * Activer/Desactiver la musique
	 */
	void EnableMusic(bool enabled);

private:
	SoundSystem();
	SoundSystem(const SoundSystem&);
	~SoundSystem();

	enum {MAX_SOUNDS = 20};

	sf::Sound sounds_[MAX_SOUNDS];
	int last_used_;
	sf::Music* music_;
	bool enable_music_;
};

#endif // SOUNDSYSTEM_HPP
