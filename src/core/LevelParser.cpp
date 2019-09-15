#include "LevelParser.hpp"
#include "Constants.hpp"
#include "Resources.hpp"
#include "utils/SFML_Helper.hpp"

#include <iostream>
#include <stdexcept>


LevelParser::LevelParser():
    m_currentLevel(1),
    m_lastUnlockedLevel(1)
{
}


void LevelParser::loadLevelFile(const std::string& path)
{
    // Open level file
    if (m_xmlDoc.LoadFile(path.c_str()) != 0) {
        std::string error = "Cannot load levels from " + path + ": " + m_xmlDoc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    // Parse level nodes
    tinyxml2::XMLElement* root = m_xmlDoc.RootElement();
    tinyxml2::XMLElement* node = root->FirstChildElement("level");
    while (node) {
        m_levels.push_back(node);
        node = node->NextSiblingElement("level");
    }
}


const char* LevelParser::getTilemapPath() const
{
    // Parse tilemap element
    const tinyxml2::XMLElement* elem = getCurrentLevelElement()->FirstChildElement("tilemap");
    return elem->Attribute("src");
}


int LevelParser::getTilemapSpeed() const
{
    return getCurrentLevelElement()->FirstChildElement("tilemap")->IntAttribute("speed");
}


void LevelParser::setCurrent(size_t level)
{
    if (level < 1 || level > m_lastUnlockedLevel || level > m_levels.size()) {
        std::cerr << " [levels] level " << level << " is undefined or still locked, using level 1 instead" << std::endl;
        level = 1;
    }
    m_currentLevel = level;
}


size_t LevelParser::getCurrent() const
{
    return m_currentLevel;
}


size_t LevelParser::unlockNextLevel()
{
    if (m_lastUnlockedLevel < m_levels.size()) {
        m_currentLevel = ++m_lastUnlockedLevel;
    }
    return m_currentLevel;
}


size_t LevelParser::getLastUnlocked() const
{
    return m_lastUnlockedLevel;
}


void LevelParser::setLastUnlocked(size_t level)
{
    if (level < 1 || level > m_levels.size()) {
        std::cerr << " [levels] level " << level << " is undefined, using level 1 instead" << std::endl;
        level = 1;
    }
    m_lastUnlockedLevel = level;
}


size_t LevelParser::getLevelCount() const
{
    return m_levels.size();
}


bool LevelParser::parseLayers(std::vector<ParallaxLayer>& layers) const
{
    const tinyxml2::XMLElement* parentElem = getCurrentLevelElement()->FirstChildElement("layers");
    if (parentElem) {
        int i = 0;
        const tinyxml2::XMLElement* elem = parentElem->FirstChildElement("layer");
        while (elem) {
            layers.resize(++i);
            ParallaxLayer& layer = layers[i - 1];

            // Texture
            const char* src = elem->Attribute("src");
            bool repeatY = true;
            elem->QueryBoolAttribute("repeat", &repeatY);
            if (src) {
                sf::Texture& texture = Resources::getTexture(src);
                texture.setRepeated(true);
                layer.setTexture(texture, repeatY);
            }

            // Speed
            int speed = 0;
            if (elem->QueryIntAttribute("speed", &speed) != tinyxml2::XML_SUCCESS) {
                std::cerr << "[LevelParser] cannot parse layer speed" << std::endl;
            }
            layer.setSpeed(speed);

            // Color + blend mode
            const char* colorCode = elem->Attribute("color");
            if (colorCode) {
                std::cout << "set color " << colorCode << " for layer " << i + 1 << std::endl;
                layer.setColor(sfh::hexa_to_color(colorCode));
            }

            elem = elem->NextSiblingElement("layer");
        }
    }
    return true;
}


int LevelParser::getStarsCount() const
{
    int nb_stars = 0;
    getCurrentLevelElement()->QueryIntAttribute("stars", &nb_stars);
    return nb_stars;
}


const char* LevelParser::getMusicName() const
{
    return getCurrentLevelElement()->Attribute("music");
}


const tinyxml2::XMLElement* LevelParser::getCurrentLevelElement() const
{
    return m_levels.at(m_currentLevel - 1);
}
