#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <map>
#include "Item.hpp"

/**
 * Singleton for storing upgrade items
 */
class ItemManager
{
public:
    static ItemManager& getInstance();

    /**
     * Load items definitions from XML
     * @param filename: XML document
     */
    void loadFromXML(const std::string& filename);

    /**
     * Check if an item exists
     */
    bool hasItem(Item::Type, int level) const;

    /**
     * Get an item
     */
    const Item& getItem(Item::Type type, int level) const;

private:
    ItemManager();

    void parseItems(tinyxml2::XMLElement* elem, const char* tagname, Item::Type type);

    typedef std::pair<Item::Type, int> ItemID; // An item is identified by Type + Level
    typedef std::map<ItemID, Item> ItemMap;
    ItemMap m_items;
};

#endif // ITEMMANAGER_HPP
