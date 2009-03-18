#include <fstream>
#include <iostream>

#include "MediaManager.hpp"

#define IMG_LIST "data/images/images.txt"
#define IMG_PATH "data/images/"

#define SOUND_LIST "data/sound/sound.txt"
#define SOUND_PATH "data/sound/"

#define MUSIC_LIST "data/music/music.txt"
#define MUSIC_PATH "data/music/"

#define FONT_FILENAME "data/font/hemi-head.ttf"


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

#ifndef NO_AUDIO
// charger un buffer audio
static void load_or_die(sf::SoundBuffer& buffer, const char* filename)
{
	std::string path(SOUND_PATH);
	if (!buffer.LoadFromFile(path + filename))
	{
		exit(EXIT_FAILURE);
	}
}


// charger un buffer lié a une instance de la lib dumb
static void load_or_die(std::string& music_name, const char* filename)
{
	music_name = filename;
}

#endif

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

#ifndef NO_AUDIO
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


Music* MediaManager::GetMusic(const char* key) const
{
	std::map<std::string, std::string>::const_iterator it;
	it = musics_.find(key);
	if (it == musics_.end())
	{
		std::cerr << "can't give you music file " << key << std::endl;
		exit(EXIT_FAILURE);
	}
	
	std::string path(MUSIC_PATH);
	Music* mus = new Music((path + it->second).c_str());
#ifdef DEBUG
	std::cout << path << it->second << " made." <<  std::endl;
#endif
	return mus;
}
#endif

const sf::Font& MediaManager::GetFont() const
{
	return font_;
}


const Animation& MediaManager::GetAnimation(const char* key) const
{
	std::map<std::string, Animation>::const_iterator it;
	it = animations_.find(key);
	if (it == animations_.end())
	{
		std::cerr << "can't give you animation " << key << std::endl;
	}
	return it->second;
}


MediaManager::MediaManager()
{
	// chargement des images
	if (!load_from_list(IMG_LIST, images_))
	{
		std::cerr << "can't open image list: " << IMG_LIST << std::endl;
		exit(EXIT_FAILURE);
	}
#ifndef NO_AUDIO
	// chargement des buffers audio
	if (!load_from_list(SOUND_LIST, sounds_))
	{
		std::cerr << "can't open sound list: " << SOUND_LIST << std::endl;
		exit(EXIT_FAILURE);
	}

	// chargement des musiques
	if (!load_from_list(MUSIC_LIST, musics_))
	{
		std::cerr << "can't open music list: " << MUSIC_LIST << std::endl;
		exit(EXIT_FAILURE);
	}
#endif
	// chargement des fontes
	if (!font_.LoadFromFile(FONT_FILENAME, 60))
	{
		exit(EXIT_FAILURE);
	}
	
	// création des animations
	BuildAnimation("capsule", 32, 32, 8, 0.1f);
	BuildAnimation("playership", 64, 40, 2, 0.1f);
	BuildAnimation("drone", 48, 32, 2, 0.1f);
}


void MediaManager::BuildAnimation(const char* name, int width, int height,
	int count, float delay, int offset)
{
	Animation* p = &animations_[name];
	for (int i = 0; i < count; ++i)
	{
		p->AddFrame(i * width, offset, width, height);
	}
	p->SetDelay(delay);
#ifdef DEBUG
	std::cout << "building anim:" << name << std::endl;
#endif
}

