#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <queue>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "vendor/tinyxml/tinyxml2.h"

class Entity;

/**
 * XML level file parser
 */
class LevelManager
{
public:
    static LevelManager& getInstance();

    /**
     * Load the XML file storing the levels definitions
     */
    void loadLevelFile(const std::string& path);

    /**
     * Parse the current level definition and allocate entities in the spawn queue
     */
    void initCurrentLevel();

    /**
     * Spawn the next entity in the current level
     * @param elapsed_time: elapsed time in seconds in the level
     * @return entity if any, or NULL
     */
    Entity* spawnNextEntity(float elapsed_time);

    /**
     * Size of the spawn queue for the current level
     * @return number of remaining entities
     */
    size_t getSpawnQueueSize() const;

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
     * Retrieve attributes from the currently loaded level
     */

    /// Bottom background image
    const sf::Texture* getBottomLayer() const;

    /// Top background image
    const sf::Texture* getTopLayer() const;

    /// Optionnal color for background image
    sf::Color getLayerColor() const;

    /// Size in pixel at the top and bottom of the screen occupied by decor
    int getDecorHeight() const;

    /// Scrolling stars quantity
    int getStarsCount() const;

    /// Background music file
    const char* getMusicName() const;

    /// Current level duration in seconds
    float getDuration() const;

    /// Total amount of points available in the current level
    int getTotalPoints() const;

private:
    LevelManager();
    LevelManager(const LevelManager&);
    ~LevelManager();

    /**
     * Recursive method for parsing entities in an XML tree
     */
    void parseEntities(const tinyxml2::XMLElement* elem);

    /**
     * Add an entity in the spawn queue
     * @param entity: entity to append
     * @param delay: time to wait since the last inserted entity
     */
    void appendToSpawnQueue(Entity* entity, float time);

    /**
     * Delete all allocated entities in the spawn queue
     */
    void resetSpawnQueue();

    /**
     * Get the XML node of the current level
     */
    const tinyxml2::XMLElement* getCurrentLevelElement() const;

    typedef std::vector<const tinyxml2::XMLElement*> NodeVector;
    typedef std::map<std::string, const tinyxml2::XMLElement*> NodeMap;

    tinyxml2::XMLDocument m_xml_doc;
    NodeVector            m_levels; // Ordered array of level nodes
    NodeMap               m_functions; // Function nodes, indexed by their name
    size_t                m_current_level;
    size_t                m_last_unlocked_level;

    // Spawn entity at spawntime
    struct EntitySlot
    {
        Entity* entity;
        float spawntime;
    };

    std::queue<EntitySlot> m_spawn_queue;
    float                  m_last_insert_time;
    int                    m_total_points;
};

#endif // LEVELMANAGER_HPP
