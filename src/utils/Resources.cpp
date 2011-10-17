#include "Resources.hpp"
#include "DumbMusic.hpp"


std::string Resources::path_ = "./data";
Resources::ImageMap Resources::images_;
Resources::FontMap Resources::fonts_;
Resources::SoundMap Resources::sounds_;
Resources::DumbMusicMap Resources::dumb_musics_;


void Resources::SetDataPath(const std::string& path)
{
	if (!path.empty())
	{
		path_ = path;
	}
}


sf::Image& Resources::GetImage(const std::string& name)
{
	ImageMap::iterator it = images_.find(name);
	if (it == images_.end())
	{
		sf::Image& img = images_[name];
		img.LoadFromFile(path_ + "/images/" + name);
		img.SetSmooth(false);
		return img;
	}
	return it->second;
}


const sf::Font& Resources::GetFont(const std::string& name, int size)
{
	FontMap::iterator it = fonts_.find(name);
	if (it == fonts_.end())
	{
		sf::Font& font = fonts_[name];
		font.LoadFromFile(path_ + "/fonts/" + name, size);
		return font;
	}
	return it->second;
}


const sf::SoundBuffer& Resources::GetSoundBuffer(const std::string& name)
{
	SoundMap::iterator it = sounds_.find(name);
	if (it == sounds_.end())
	{
		sf::SoundBuffer& sound = sounds_[name];
		sound.LoadFromFile(path_ + "/sound/" + name);
		return sound;
	}
	return it->second;
}


DumbMusic* Resources::GetDumbMusic(const std::string& name)
{
	DumbMusicMap::iterator it = dumb_musics_.find(name);
	if (it == dumb_musics_.end())
	{
		DumbMusic* music = new DumbMusic();
		music->OpenFromFile(path_ + "/music/" + name);
		dumb_musics_[name] = music;
		return music;
	}
	return it->second;
}


void Resources::Unload()
{
	for (DumbMusicMap::iterator it = dumb_musics_.begin(); it != dumb_musics_.end(); ++it)
	{
		delete it->second;
	}
	dumb_musics_.clear();
}
