#include <iostream>
#include "Item.hpp"
#include "entities/Player.hpp"
#include "utils/I18n.hpp"
#include "vendor/tinyxml/tinyxml2.h"


Item::Item(Type type):
    m_type(type),
    m_level(0),
    m_price(0)
{
}


const char* Item::typeToString(Type type)
{
    switch (type)
    {
        case Item::ENGINE:   return "item.engine";
        case Item::HULL:     return "item.hull";
        case Item::SHIELD:   return "item.shield";
        case Item::HEATSINK: return "item.heatsink";
        case Item::WEAPON:   return "item.laser";
        default: break;
    }
    return "";
}


sf::String Item::toString() const
{
    return _t(typeToString(m_type));
}


void Item::loadFromXmlNode(tinyxml2::XMLElement* elem)
{
    // Shared attributes
    elem->QueryIntAttribute("price", &m_price);
    elem->QueryIntAttribute("level", &m_level);

    // Type-specefic attributes
    switch (getType())
    {
        case Item::HULL:
            if (elem->QueryIntAttribute("hp", &m_data.hp) != 0)
                std::cerr << "XML error: 'hull.hp' is missing" << std::endl;
            break;

        case Item::ENGINE:
            if (elem->QueryFloatAttribute("speed", &m_data.speed) != 0)
                std::cerr << "XML error: 'engine.speed' is missing" << std::endl;
            break;

        case Item::SHIELD:
            if (elem->QueryIntAttribute("points", &m_data.shield) != 0)
                std::cerr << "XML error: 'shield.points' is missing" << std::endl;
            break;

        case Item::HEATSINK:
            if (elem->QueryIntAttribute("heat", &m_data.heat) != 0)
                std::cerr << "XML error: 'heatsink.heat' is missing" << std::endl;
            break;

        case Item::WEAPON:
            if (elem->QueryFloatAttribute("heatcost", &m_data.weapon.heatcost) != 0)
                std::cerr << "XML error: 'weapon.heatcost' is missing" << std::endl;

            if (elem->QueryIntAttribute("damage", &m_data.weapon.damage) != 0)
                std::cerr << "XML error 'weapon.damage' is missing" << std::endl;
            break;

        default:
            break;
    }
}


Item::Type Item::getType() const
{
    return m_type;
}


int Item::getLevel() const
{
    return m_level;
}


int Item::getPrice() const
{
    return m_price;
}


sf::String Item::getDescription() const
{
    switch (m_type)
    {
        case Item::HEATSINK:
            return I18n::templatize("item.heatsink_info", "{heat}", m_data.heat);

        case Item::HULL:
            return I18n::templatize("item.hull_info", "{hp}", m_data.hp);

        case Item::ENGINE:
            return I18n::templatize("item.engine_info", "{speed}", (int) m_data.speed);

        case Item::SHIELD:
            return I18n::templatize("item.shield_info", "{points}", m_data.shield);

        case Item::WEAPON:
            return I18n::templatize("item.laser_info", "{dmg}", m_data.weapon.damage);

        default:
            break;
    }
    return "";
}


void Item::equip(Player& player) const
{
    switch (m_type)
    {
        case Item::HEATSINK:
            player.setMaxHeat(m_data.heat);
            break;

        case Item::HULL:
            player.setMaxHP(m_data.hp);
            break;

        case Item::ENGINE:
            player.setSpeed(m_data.speed);
            break;

        case Item::SHIELD:
            player.setMaxShield(m_data.shield);
            break;

        case Item::WEAPON:
        {
            Weapon& weapon = player.getLaser();
            weapon.setDamage(m_data.weapon.damage);
            weapon.setHeatCost(m_data.weapon.heatcost);
            break;
        }
        default:
            break;
    }
}
