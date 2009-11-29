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

	/**
	 * Indiquer le volume du son
	 */
	void SetSoundVolume(float volume);

	const std::string& GetMusicName() const;
	void SetMusicName(const std::string& music_name);

	/**
	 * Jouer une musique (une musique à la fois)
	 * @param music: musique à jouer
	 * @param force_enable: si true, force l'activation de la musique avant de jouer
	 */
	void PlayMusic(const std::string& music_name, bool force_enable = false);

	/**
	 * Stopper la lecture de la musique
	 */
	void StopMusic();

	/**
	 * Indiquer le volume de la musique
	 */
	void SetMusicVolume(float volume);

	/**
	 * Activer/Desactiver la musique
	 */
	void EnableMusic(bool enabled);

	bool IsMusicEnabled() const;

private:
	SoundSystem();
	SoundSystem(const SoundSystem&);
	~SoundSystem();

	enum {MAX_SOUNDS = 20};

	sf::Sound sounds_[MAX_SOUNDS];
	int last_used_;
	sf::SoundStream* music_;
	std::string music_name_;
	float music_volume_;
	bool enable_music_;
};

#endif // SOUNDSYSTEM_HPP
