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
	void SetSoundVolume(int volume);
	inline int GetSoundVolume() const { return sound_volume_; }

	const std::string& GetMusicName() const;
	void SetMusicName(const std::string& music_name);

	/**
	 * Jouer une musique (une musique à la fois)
	 * @param music: musique à jouer
	 */
	void PlayMusic(const std::string& music_name);

	/**
	 * Stopper la lecture de la musique
	 */
	void StopMusic();

	/**
	 * Indiquer le volume de la musique
	 */
	void SetMusicVolume(int volume);
	inline int GetMusicVolume() const { return music_volume_; }

	/**
	 * Activer/Desactiver la musique
	 */
	void EnableMusic(bool enabled);
	void EnableSound(bool enabled);
	bool IsMusicEnabled() const;
	bool IsSoundEnabled() const;

	/**
	 * Arrêter la lecture de tous les fichier en cours
	 */
	void StopAll();

private:
	SoundSystem();
	SoundSystem(const SoundSystem&);
	~SoundSystem();

	enum {MAX_SOUNDS = 20};

	sf::Sound sounds_[MAX_SOUNDS];
	int last_used_;
	sf::SoundStream* music_;
	std::string music_name_;
	int music_volume_;
	int sound_volume_;
	bool enable_music_;
	bool enable_sound_;
};

#endif // SOUNDSYSTEM_HPP
