#ifndef ITEM_HPP
#define ITEM_HPP

#include <SFML/System/String.hpp>

namespace tinyxml2
{
class XMLElement;
}

class Player;

/**
 * Represents an item which can be equiped by the player
 */
class Item
{
public:
    enum Type
    {
        HULL,
        ENGINE,
        SHIELD,
        HEATSINK,
        WEAPON,
        _COUNT,
    };

    Item(Type type);

    static const char* typeToString(Type type);

    sf::String toString() const;

    void loadFromXmlNode(tinyxml2::XMLElement* elem);

    Type getType() const;

    int getLevel() const;

    int getPrice() const;

    sf::String getDescription() const;

    void equip(Player& player) const;

private:
    union Data
    {
        int shield;
        float speed;
        int hp;
        int heat;
        struct weapon_t
        {
            float heatcost;
            int   damage;
        } weapon;
    };

    Data m_data;
    Type m_type;
    int m_level;
    int m_price;
};

#endif // ITEM_HPP
