#ifndef RESOURCES_HPP
#define RESOURCES_HPP

#include <string>
#include <map>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

/**
 * Static class for loading and storing resources
 */
class Resources
{
public:
    /**
     * Set path where resources are located
     */
    static void setSearchPath(const std::string& path);

    /**
     * Get path where resources are located
     */
    static const std::string& getSearchPath();

    /**
     * Get a texture from the 'images' directory
     * @param name: texture filename
     */
    static sf::Texture& getTexture(const std::string& name);

    /**
     * Get a font from the 'fonts' directory
     * @param name: font filename
     */
    static sf::Font& getFont(const std::string& name);

    /**
     * Get a sound buffer from the 'sounds' directory
     * @param name: sound buffer filename
     */
    static sf::SoundBuffer& getSoundBuffer(const std::string& name);

private:
    static std::string m_path;

    typedef std::map<std::string, sf::Texture> TextureMap;
    static TextureMap m_textures;

    typedef std::map<std::string, sf::Font> FontMap;
    static FontMap m_fonts;

    typedef std::map<std::string, sf::SoundBuffer> SoundMap;
    static SoundMap m_sounds;
};

#endif // RESOURCES_HPP
