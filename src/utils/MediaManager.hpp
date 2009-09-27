#ifndef MEDIAMANAGER_HPP
#define MEDIAMANAGER_HPP

#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#ifndef NO_DUMB_MUSIC
#include "../utils/DumbMusic.hpp"
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
	 * @param key: identifiant de l'image
	 * @return référence sur l'image
	 */
	const sf::Image& GetImage(const char* key) const;


	/**
	 * Obtenir un buffer audio
	 * @param key: identifiant du buffer
	 * @return référence sur le buffer
	 */
	const sf::SoundBuffer& GetSoundBuf(const char* key) const;

#ifndef NO_DUMB_MUSIC
	/**
	 * Obtenir une musique
	 * @param[in] key: identifiant de la musique
	 * @return pointeur sur la musique
	 */
	DumbMusic* GetDumbMusic(const char* key) const;
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
	std::map<std::string, sf::SoundBuffer> sounds_;
#ifndef NO_DUMB_MUSIC
	std::map<std::string, std::string> musics_;
#endif
	sf::Font font_;
};


// fonctions inline pour alléger la syntaxe

inline const sf::Image& GET_IMG(const char* key)
{
	return MediaManager::GetInstance().GetImage(key);
}

inline const sf::SoundBuffer& GET_SOUNDBUF(const char* key)
{
	return MediaManager::GetInstance().GetSoundBuf(key);
}

#ifndef NO_DUMB_MUSIC
inline DumbMusic* GET_DUMB_MUSIC(const char* key)
{
	return MediaManager::GetInstance().GetDumbMusic(key);
}
#endif

inline const sf::Font& GET_FONT()
{
	return MediaManager::GetInstance().GetFont();
}

#endif // MEDIAMANAGER_HPP

