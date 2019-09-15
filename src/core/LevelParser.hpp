#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "core/ParallaxLayer.hpp"
#include "vendor/tinyxml/tinyxml2.h"

#include <unordered_map>
#include <string>
#include <SFML/Graphics.hpp>

class Entity;

/**
 * XML level file parser
 */
class LevelParser {
public:
    LevelParser();

    /**
     * Load the XML file storing the levels definitions
     */
    void loadLevelFile(const std::string& path);

    sf::Vector2u getTilemapSize() const;

    /**
     * Set the current level
     * @param level: level number, between 1 and getLastUnlocked()
     */
    void setCurrent(size_t level_num);
    size_t getCurrent() const;

    /**
     * Last reached level
     * @param level: level number, between 1 and getLevelCount()
     */
    size_t unlockNextLevel();
    void setLastUnlocked(size_t level);
    size_t getLastUnlocked() const;

    /**
     * Get number of levels available
     */
    size_t getLevelCount() const;

    /**
     * Parse list of layers
     */
    bool parseLayers(std::vector<ParallaxLayer>& layers) const;

    /**
     * Get current level.stars attribute
     */
    int getStarsCount() const;

    /**
     * Get current level.music attribute
     */
    const char* getMusicName() const;

    /**
     * Get current level.tilemap.src attribute
     */
    const char* getTilemapPath() const;

    /**
     * Get current level.tilemap.speed attribute
     */
    int getTilemapSpeed() const;

private:
    /**
     * Get the XML node of the current level
     */
    const tinyxml2::XMLElement* getCurrentLevelElement() const;

    typedef std::vector<const tinyxml2::XMLElement*> NodeVector;
    typedef std::unordered_map<std::string, const tinyxml2::XMLElement*> NodeMap;

    tinyxml2::XMLDocument m_xmlDoc;
    NodeVector            m_levels; // Ordered array of level nodes
    size_t                m_currentLevel;
    size_t                m_lastUnlockedLevel;
};

#endif
