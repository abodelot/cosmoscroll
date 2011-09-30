#ifndef MEDIAMANAGER_HPP
#define MEDIAMANAGER_HPP

#include <map>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#ifndef NO_DUMB_MUSIC
#include "DumbMusic.hpp"
#endif

#include "MediaNotFoundException.hpp"

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

	static const sf::Font& GetFont(const char* key, int size = 12);

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
	 * @param key: identifiant de la musique
	 * @return pointeur sur la musique
	 */
	DumbMusic* GetDumbMusic(const char* key) const;
#endif

	/**
	 * Appliquer ou annuler le filtre smooth sur une image
	 * @param key: identifiant de l'image
	 */
	void SmoothImage(const char* key, bool smooth);

	void CreateImageMask(const char* key, const sf::Color& color);

	void Unload();

private:
	/**
	 * Initialisation (chargement des images)
	 */
	MediaManager();
	~MediaManager();
	MediaManager(const MediaManager&);
	MediaManager& operator=(const MediaManager&);

	std::map<std::string, sf::Image> images_;
	std::map<std::string, sf::SoundBuffer> sounds_;
	std::map<std::string, sf::Font> fonts_;

#ifndef NO_DUMB_MUSIC
	typedef std::map<std::string, DumbMusic*> DumbMusicMap;
	DumbMusicMap musics_;
#endif
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


#endif // MEDIAMANAGER_HPP

