#ifndef SOUNDSYSTEM_HPP
#define SOUNDSYSTEM_HPP

#include <string>
#include <SFML/Audio.hpp>

class IniParser;

class SoundSystem
{
public:
	static SoundSystem& GetInstance();

	/**
	 * Jouer une musique (une musique à la fois)
	 * @param music: musique à jouer
	 */
	void PlayMusic(const std::string& music_name);

	/**
	 * Charger une musique
	 */
	void SetMusic(const std::string& music_name);
	const std::string& GetMusicName() const;

	/**
	 * Stopper la lecture de la musique
	 */
	void StopMusic();

	/**
	 * Jouer un son
	 */
	void PlaySound(const sf::SoundBuffer& soundbuffer);

	/**
	 * Volume de la musique
	 */
	void SetMusicVolume(int volume);
	inline int GetMusicVolume() const { return music_volume_; }

	/**
	 * Volume des sons
	 */
	void SetSoundVolume(int volume);
	inline int GetSoundVolume() const { return sound_volume_; }

	/**
	 * Activer/désactiver la musique
	 */
	void EnableMusic(bool enabled);
	bool IsMusicEnabled() const;

	/**
	 * Activer/désactiver les sons
	 */
	void EnableSound(bool enabled);
	bool IsSoundEnabled() const;

	/**
	 * Arrêter la lecture de tous les ressources utilisées
	 */
	void StopAll();

	void LoadFromConfig(IniParser& config);
	void SaveToConfig(IniParser& config);

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
