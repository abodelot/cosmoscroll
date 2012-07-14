#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <map>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class DumbMusic;

/**
 * Static class for loading resources
 */
class Resources
{
public:
	// Set path from which resources are loaded
	static void SetDataPath(const std::string& path);
	static const std::string& GetDataPath();

	// Load an image
	static sf::Image& GetImage(const std::string& name);

	// Load a font
	static const sf::Font& GetFont(const std::string& name, int size=12);

	// Load a sound buffer
	static const sf::SoundBuffer& GetSoundBuffer(const std::string& name);

private:
	static std::string path_;

	typedef std::map<std::string, sf::Image> ImageMap;
	static ImageMap images_;

	typedef std::map<std::string, sf::Font> FontMap;
	static FontMap fonts_;

	typedef std::map<std::string, sf::SoundBuffer> SoundMap;
	static SoundMap sounds_;
};

#endif // RESOURCES_HPP
