#ifndef H_RESSOURCE_MANAGER
#define H_RESSOURCE_MANAGER

#include <SFML/Graphics.hpp>
#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#ifndef NO_MUSIC
#include "Music.hpp"
#endif
#endif
#include <map>
#include <string>

/*
 * Classe singleton pour gérer les ressources (images, font)
 */
class MediaManager
{
public:
    /*
     * Récupérer l'instance unique
     * @return référence sur le gestionnaire de ressources
     */
    static MediaManager& GetInstance();
    
    /*
     * Récupérer une image
     * @param[in] key: identifiant de l'image
     * @return référence sur l'image
     */
    const sf::Image& GetImage(const char* key) const;

#ifndef NO_AUDIO    
    /*
     * Récupérer un buffer audio
     * @param[in] key: identifiant du buffer
     * @return référence sur le buffer
     */
    const sf::SoundBuffer& GetSoundBuf(const char* key) const;

#ifndef NO_MUSIC
    /*
     * Récupérer une musique
     * @param[in] key: identifiant de la musique
     * @return pointeur sur la musique
     */
	Music* GetMusic(const char* key) const;
	
#endif
	
#endif
    
    /*
     * Récupérer la police de caractères
     * @param[in] size: taille énuméree
     * @return référence sur la police
     */
    const sf::Font& GetFont() const;
    
private:
    /*
     * Initialisation (chargement des images)
     */
    MediaManager();
    
    MediaManager(const MediaManager& other);
    MediaManager& operator=(const MediaManager& other);
    
    std::map<std::string, sf::Image> images_;
#ifndef NO_AUDIO
    std::map<std::string, sf::SoundBuffer> sounds_;

#ifndef NO_MUSIC
	std::map<std::string, std::string> musics_;
#endif
	
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

#ifndef NO_MUSIC
inline Music* GET_MUSIC(const char* key)
{
    return MediaManager::GetInstance().GetMusic(key);
}
#endif

#endif

inline const sf::Font& GET_FONT()
{
    return MediaManager::GetInstance().GetFont();
}

#endif /* guard H_RESSOURCE_MANAGER */

