#include "MediaManager.hpp"

#include <fstream>
#include <iostream>

#define IMG_LIST "images/images.txt"
#define IMG_PATH "images/"

#define SOUND_LIST "sound/sound.txt"
#define SOUND_PATH "sound/"


// charger une image
static void load_or_die(sf::Image& image, const char* filename)
{
    std::string path(IMG_PATH);
    if (!image.LoadFromFile(path + filename))
    {
        exit(EXIT_FAILURE);
    }
    image.SetSmooth(false);
}

// charger un buffer audio
static void load_or_die(sf::SoundBuffer& buffer, const char* filename)
{
    std::string path(SOUND_PATH);
    if (!buffer.LoadFromFile(path + filename))
    {
        exit(EXIT_FAILURE);
    }
}

// charger une liste de ressources depuis un fichier
template <typename Ressource>
static bool load_from_list(const char* filelist, std::map<std::string, Ressource>& table)
{
    std::ifstream f(filelist);
    if (f)
    {
        std::string line;
        while (getline(f, line))
        {
#ifdef DEBUG
            std::cout << "loading: " << line << std::endl;
#endif
            // la clef de la ressource dans la map est le nom du fichier
            // sans son extension
            size_t found = line.find_last_of('.');
            if (found != std::string::npos)
            {
                std::string key = line.substr(0, found);
                if (key != "")
                {
                    load_or_die(table[key], line.c_str());
                }
                else
                {
                    std::cerr << "invalid ressource name:" << line << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }
        f.close();
        return true;
    }
    return false;
}


MediaManager& MediaManager::GetInstance()
{
    static MediaManager self;
    return self;
}


const sf::Image& MediaManager::GetImage(const char* image) const
{
    std::map<std::string, sf::Image>::const_iterator it;
    it = images_.find(image);
    if (it == images_.end())
    {
        std::cerr << "can't give you image " << image << std::endl;
        exit(EXIT_FAILURE);
    }
    return it->second;
}


const sf::SoundBuffer& MediaManager::GetSoundBuf(const char* key) const
{
    std::map<std::string, sf::SoundBuffer>::const_iterator it;
    it = sounds_.find(key);
    if (it == sounds_.end())
    {
        std::cerr << "can't give you sound buffer " << key << std::endl;
        exit(EXIT_FAILURE);
    }
    return it->second;
}


const sf::Font& MediaManager::GetFont() const
{
    return font_;
}


MediaManager::MediaManager()
{
    // chargement des images
    if (!load_from_list(IMG_LIST, images_))
    {
        std::cerr << "can't open image list: " << IMG_LIST << std::endl;
        exit(EXIT_FAILURE);
    }
    
    // chargement des buffers audio
    if (!load_from_list(SOUND_LIST, sounds_))
    {
        std::cerr << "can't open sound list: " << SOUND_LIST << std::endl;
        exit(EXIT_FAILURE);
    }
    // chargement des fontes
    if (!font_.LoadFromFile("font/hemi-head.ttf", 60))
	{
	    exit(EXIT_FAILURE);
    }
}

