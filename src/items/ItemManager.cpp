#include <stdexcept>
#include <iostream>
#include "ItemManager.hpp"
#include "utils/tinyxml/tinyxml2.h"


ItemManager& ItemManager::getInstance()
{
    static ItemManager self;
    return self;
}


ItemManager::ItemManager()
{
}


void ItemManager::loadFromXML(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0)
    {
        std::string error = "Cannot load items from '" + filename + "': " + doc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    tinyxml2::XMLElement* root = doc.RootElement();
    parseItems(root, "weapons",   Item::WEAPON);
    parseItems(root, "shields",   Item::SHIELD);
    parseItems(root, "heatsinks", Item::HEATSINK);
    parseItems(root, "hulls",     Item::HULL);
    parseItems(root, "engines",   Item::ENGINE);
}


bool ItemManager::hasItem(Item::Type type, int level) const
{
    ItemMap::const_iterator it = m_items.find(ItemID(type, level));
    return it != m_items.end();
}


const Item& ItemManager::getItem(Item::Type type, int level) const
{
    ItemMap::const_iterator it = m_items.find(ItemID(type, level));
    if (it == m_items.end())
        throw std::runtime_error("Item <type:" + std::string(Item::typeToString(type)) + ", lvl:" + std::to_string(level) + "> not found");

    return it->second;
}


void ItemManager::parseItems(tinyxml2::XMLElement* elem, const char* tagname, Item::Type type)
{
    elem = elem->FirstChildElement(tagname);
    if (elem != NULL)
    {
        elem = elem->FirstChildElement();
        while (elem != NULL)
        {
            Item item(type);
            item.loadFromXmlNode(elem);
            // Store item, indexed Type + Level
            ItemID id(type, item.getLevel());
            m_items.insert(std::pair<ItemID, Item>(id, item));
            elem = elem->NextSiblingElement();
        }
    }
    else
    {
        std::cerr << "XML error: tag '" << tagname << "' not found" << std::endl;
    }
}
