#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <map>

class Animation;
class Weapon;
namespace tinyxml2 {
    class XMLElement;
}

#include "items/Item.hpp"
#include "entities/Spaceship.hpp"

/**
 * Load game object definitions from XML documents and index them
 */
class Factory {
public:
    /**
     * Load animation definitions
     * @param path: path to XML document
     */
    void loadAnimationsFromXml(const std::string& path);

    /**
     * Get an animation
     * @param id: animation ID from XML document
     */
    const Animation& getAnimation(const std::string& id) const;

    /**
     * Load weapon definitions
     * @param path path to XML document
     */
    void loadWeaponsFromXml(const std::string& path);

    /**
     * Get a weapon
     * @param id: weapon ID from XML document
     */
    const Weapon& getWeapon(const std::string& id) const;

    /**
     * Load spaceship definitions
     * @param path: path to XML document
     */
    void loadSpaceshipsFromXml(const std::string& path);

    /**
     * Allocate a new Spaceship with a given profile
     * @param colorId: color serving as ID for spaceship, from XML document
     * @return new allocated spaceship
     */
    Spaceship* createSpaceship(int colorId) const;

    /**
     * Load items definitions from XML
     * @param path: path XML document
     */
    void loadItemsFromXml(const std::string& path);

    /**
     * Check if item with given properties exists
     */
    bool hasItem(Item::Type, int level) const;

    /**
     * Get an item
     */
    const Item& getItem(Item::Type type, int level) const;

private:
    typedef std::map<std::string, Animation> AnimationMap;
    AnimationMap m_animations;

    typedef std::map<std::string, Weapon> WeaponMap;
    WeaponMap m_weapons;

    typedef std::map<int, Spaceship> SpaceshipMap;
    SpaceshipMap m_spaceships;

    void parseItems(tinyxml2::XMLElement* elem, const char* tagname, Item::Type type);

    typedef std::pair<Item::Type, int> ItemID; // An item is identified by Type + Level
    typedef std::map<ItemID, Item> ItemMap;
    ItemMap m_items;
};

#endif
