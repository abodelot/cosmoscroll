#include "Resources.hpp"
#include "DumbMusic.hpp"


std::string Resources::path_ = "./data";
Resources::ImageMap Resources::images_;
Resources::FontMap Resources::fonts_;
Resources::SoundMap Resources::sounds_;


void Resources::SetDataPath(const std::string& path)
{
	if (!path.empty())
	{
		path_ = path;
	}
}


const std::string& Resources::GetDataPath()
{
	return path_;
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
