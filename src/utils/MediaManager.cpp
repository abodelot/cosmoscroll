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

// charger un buffer audio
static void load_or_die(sf::SoundBuffer& buffer, const char* filename)
{
	std::string path(SOUND_PATH);
	if (!buffer.LoadFromFile(path + filename))
	{
		exit(EXIT_FAILURE);
	}
}

#ifndef NO_DUMB_MUSIC
// charger une musique avec la libdumb
static void load_or_die(DumbMusic*& music, const char* filename)
{
	std::string path(MUSIC_PATH);
	music = new DumbMusic((path + filename).c_str());
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


MediaManager::MediaManager()
{
	// chargement des images
#ifdef DEBUG
	puts("* loading images...");
#endif
	if (!load_from_list(IMG_LIST, images_))
	{
		std::cerr << "can't open image list: " << IMG_LIST << std::endl;
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	puts("* loading sounds...");
#endif
	// chargement des buffers audio
	if (!load_from_list(SOUND_LIST, sounds_))
	{
		std::cerr << "can't open sound list: " << SOUND_LIST << std::endl;
		exit(EXIT_FAILURE);
	}
#ifdef DEBUG
	puts("* loading musics...");
#endif
#ifndef NO_DUMB_MUSIC
	DumbMusic::Init();
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
}


MediaManager::~MediaManager()
{
	Unload();
	DumbMusic::Exit();
}


const sf::Image& MediaManager::GetImage(const char* key) const
{
	std::map<std::string, sf::Image>::const_iterator it = images_.find(key);
	if (it == images_.end())
	{
		throw MediaNotFoundException(key);
	}
	return it->second;
}


const sf::SoundBuffer& MediaManager::GetSoundBuf(const char* key) const
{
	std::map<std::string, sf::SoundBuffer>::const_iterator it = sounds_.find(key);
	if (it == sounds_.end())
	{
		throw MediaNotFoundException(key);
	}
	return it->second;
}

#ifndef NO_DUMB_MUSIC
DumbMusic* MediaManager::GetDumbMusic(const char* key) const
{
	DumbMusicMap::const_iterator it = musics_.find(key);
	if (it == musics_.end())
	{
		throw MediaNotFoundException(key);
	}
	return it->second;
}
#endif

const sf::Font& MediaManager::GetFont() const
{
	return font_;
}


void MediaManager::SmoothImage(const char* key, bool smooth)
{
	images_[key].SetSmooth(smooth);
}


void MediaManager::Unload()
{
	#ifndef NO_DUMB_MUSIC
	DumbMusicMap::iterator it = musics_.begin();
	for (; it != musics_.end(); ++it)
	{
		delete it->second;
	}
	#endif
}
