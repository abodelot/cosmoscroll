#ifndef H_RESSOURCE_MANAGER
#define H_RESSOURCE_MANAGER

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <string>

/*
 * Classe singleton pour gérer les ressources (images, font)
 */
class RessourceManager
{
public:
    /*
     * Récupérer l'instance unique
     * @return référence sur le gestionnaire de ressources
     */
    static RessourceManager& GetInstance();
    
    /*
     * Récupérer une image
     * @param[in] key: identifiant de l'image
     * @return référence sur l'image
     */
    const sf::Image& GetImage(const char* key) const;
    
    /*
     * Récupérer un buffer audio
     * @param[in] key: identifiant du buffer
     * @return référence sur le buffer
     */
    const sf::SoundBuffer& GetSoundBuf(const char* key) const;
    
    /*
     * Récupérer la police
     * @return référence sur la police
     */
    const sf::Font& GetFont() const;
    
private:
    /*
     * Initialisation (chargement des images)
     */
    RessourceManager();
    
    RessourceManager(const RessourceManager& other);
    RessourceManager& operator=(const RessourceManager& other);
    
    std::map<std::string, sf::Image> images_;
    std::map<std::string, sf::SoundBuffer> sounds_;
    sf::Font font_;
};


// fonctions inline pour alléger la syntaxe

inline const sf::Image& GET_IMG(const char* key)
{
    return RessourceManager::GetInstance().GetImage(key);
}

inline const sf::SoundBuffer& GET_SOUNDBUF(const char* key)
{
    return RessourceManager::GetInstance().GetSoundBuf(key);
}

inline const sf::Font& GET_FONT()
{
    return RessourceManager::GetInstance().GetFont();
}

#endif /* guard H_RESSOURCE_MANAGER */

