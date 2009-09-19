#ifndef MEDIAMANAGER_HPP
#define MEDIAMANAGER_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#include "../utils/Music.hpp"
#endif

/**
 * Classe singleton pour gérer les ressources (images, font)
 */
class MediaManager
{
public:
	/**
	 * Obtenir l'instance unique
	 * @return référence sur le gestionnaire de ressources
	 */
	static MediaManager& GetInstance();

	/**
	 * Obtenir une image
	 * @param[in] key: identifiant de l'image
	 * @return référence sur l'image
	 */
	const sf::Image& GetImage(const char* key) const;

#ifndef NO_AUDIO
	/**
	 * Obtenir un buffer audio
	 * @param[in] key: identifiant du buffer
	 * @return référence sur le buffer
	 */
	const sf::SoundBuffer& GetSoundBuf(const char* key) const;

	/**
	 * Obtenir une musique
	 * @param[in] key: identifiant de la musique
	 * @return pointeur sur la musique
	 */
	Music* GetMusic(const char* key) const;
#endif

	/**
	 * Obtenir la police de caractères
	 * @return référence sur la police
	 */
	const sf::Font& GetFont() const;

	/**
	 * Appliquer ou annuler le filtre smooth sur une image
	 * @param key: identifiant de l'image
	 */
	void SmoothImage(const char* key, bool smooth);

private:
	/**
	 * Initialisation (chargement des images)
	 */
	MediaManager();
	MediaManager(const MediaManager&);
	MediaManager& operator=(const MediaManager&);

	std::map<std::string, sf::Image> images_;
#ifndef NO_AUDIO
	std::map<std::string, sf::SoundBuffer> sounds_;
	std::map<std::string, std::string> musics_;
#endif
	sf::Font font_;
};


// fonctions inline pour alléger la syntaxe

inline const sf::Image& GET_IMG(const char* key)
{
	return MediaManager::GetInstance().GetImage(key);
}

#ifndef NO_AUDIO
inline const sf::SoundBuffer& GET_SOUNDBUF(const char* key)
{
	return MediaManager::GetInstance().GetSoundBuf(key);
}

inline Music* GET_MUSIC(const char* key)
{
	return MediaManager::GetInstance().GetMusic(key);
}
#endif

inline const sf::Font& GET_FONT()
{
	return MediaManager::GetInstance().GetFont();
}

#endif // MEDIAMANAGER_HPP

