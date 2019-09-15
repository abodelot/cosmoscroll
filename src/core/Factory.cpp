#include "Factory.hpp"
#include "Resources.hpp"
#include "Collisions.hpp"
#include "core/Services.hpp"
#include "entities/Spaceship.hpp"
#include "vendor/tinyxml/tinyxml2.h"

#include <iostream>
#include <stdexcept>

/**
 * Get attack pattern encoded in an xml element
 */
static Spaceship::AttackPattern parse_attack_pattern(const tinyxml2::XMLElement* elem)
{
    if (elem->Attribute("attack", "auto_aim")) return Spaceship::AUTO_AIM;
    if (elem->Attribute("attack", "on_sight")) return Spaceship::ON_SIGHT;
    if (elem->Attribute("attack", "none")    ) return Spaceship::NONE;

    if (elem->Attribute("attack") != nullptr) {
        std::cerr << "[Factory] unknown attack pattern: " << elem->Attribute("attack") << std::endl;
    }
    return Spaceship::NONE;
}

/**
 * Get movement pattern encoded in an xml element
 */
static Spaceship::MovementPattern parse_movement_pattern(const tinyxml2::XMLElement* elem)
{
    if (elem->Attribute("move", "line")  ) return Spaceship::LINE;
    if (elem->Attribute("move", "magnet")) return Spaceship::MAGNET;
    if (elem->Attribute("move", "sinus") ) return Spaceship::SINUS;
    if (elem->Attribute("move", "circle")) return Spaceship::CIRCLE;

    if (elem->Attribute("move") != nullptr) {
        std::cerr << "[Factory] unknown movement pattern: " << elem->Attribute("move") << std::endl;
    }
    return Spaceship::LINE;
}

// Animations
//------------------------------------------------------------------------------

void Factory::loadAnimationsFromXml(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0) {
        std::string error = "Cannot load animations from " + filename + ": " + doc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement("anim");
    while (elem != nullptr) {
        // Parse attributes
        bool ok = true;
        const char* name;
        ok &= ((name = elem->Attribute("name")) != nullptr);
        const char* img;
        ok &= ((img = elem->Attribute("img")) != nullptr);
        int width, height, count;
        ok &= (elem->QueryIntAttribute("width", &width) == tinyxml2::XML_SUCCESS);
        ok &= (elem->QueryIntAttribute("height", &height) == tinyxml2::XML_SUCCESS);
        ok &= (elem->QueryIntAttribute("count", &count) == tinyxml2::XML_SUCCESS);
        float delay = 0.f;
        ok &= (elem->QueryFloatAttribute("delay", &delay) == tinyxml2::XML_SUCCESS);

        int x = 0, y = 0;
        elem->QueryIntAttribute("x", &x);
        elem->QueryIntAttribute("y", &y);

        if (ok) {
            // Create animation and add frames
            Animation& animation = m_animations[name];
            for (int i = 0; i < count; ++i) {
                animation.addFrame({x + i * width, y, width, height});
            }
            animation.setDelay(delay);
            animation.setTexture(Resources::getTexture(img));
            Services::getCollisions().registerTexture(&animation.getTexture());
        }
        elem = elem->NextSiblingElement("anim");
    }
}


const Animation& Factory::getAnimation(const std::string& id) const
{
    AnimationMap::const_iterator it = m_animations.find(id);
    if (it == m_animations.end()) {
        throw std::runtime_error("Animation '" + id + "' not found");
    }
    return it->second;
}


void Factory::loadWeaponsFromXml(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0) {
        std::string error = "Cannot load weapons from " + filename + ": " + doc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    // Loop over 'weapon' tags
    tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement("weapon");
    while (elem != nullptr) {
        // Weapon ID
        const char* p = elem->Attribute("id");
        if (!p) {
            throw std::runtime_error("XML error: weapon.id is missing");
        }
        Weapon& weapon = m_weapons[p];

        p = elem->Attribute("image"); // Projectile image
        if (p)
            weapon.setTexture(&Resources::getTexture(p));
        else
            std::cerr << "XML error: weapon.image is missing" << std::endl;

        p = elem->Attribute("sound"); // Sound effect
        if (p)
            weapon.setSound(&Resources::getSoundBuffer(p));
        else
            std::cerr << "XML error: weapon.sound is missing" << std::endl;

        float heatcost = 0.f;
        if (elem->QueryFloatAttribute("heatcost", &heatcost) == 0)
            weapon.setHeatCost(heatcost);
        else
            std::cerr << "XML error: weapon.heatcost is missing" << std::endl;

        float firerate = 0.f;
        if (elem->QueryFloatAttribute("firerate", &firerate) == 0)
            weapon.setFireRate(firerate);
        else
            std::cerr << "XML error: weapon.firerate is missing" << std::endl;

        int damage = 0;
        if (elem->QueryIntAttribute("damage", &damage) == 0)
            weapon.setDamage(damage);
        else
            std::cerr << "XML error: weapon.damage is missing" << std::endl;

        int speed = 0;
        if (elem->QueryIntAttribute("speed", &speed) == 0)
            weapon.setVelociy(speed);
        else
            std::cerr << "XML error: weapon.speed is missing" << std::endl;

        elem = elem->NextSiblingElement("weapon");
    }
}


const Weapon& Factory::getWeapon(const std::string& id) const
{
    WeaponMap::const_iterator it = m_weapons.find(id);
    if (it == m_weapons.end()) {
        throw std::runtime_error("Weapon '" + id + "' not found");
    }
    return it->second;
}


void Factory::loadSpaceshipsFromXml(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0) {
        std::string error = "Cannot load spaceships from " + filename + ": " + doc.GetErrorStr1();
        std::cerr << doc.GetErrorStr1() << std::endl;
        std::cerr << doc.GetErrorStr2() << std::endl;
        throw std::runtime_error(error);
    }

    tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement("spaceship");
    while (elem != nullptr) {
        const char* id = elem->Attribute("id");
        if (id == nullptr) {
            throw std::runtime_error("XML parse error: spaceship.id is missing");
        }
        // Parse id as hex color
        int colorId = strtol(id, nullptr, 16);

        const char* animation = elem->Attribute("animation");
        if (animation == nullptr) {
            throw std::runtime_error("XML error: spaceship.animation is missing");
        }
        int hp = 0;
        if (elem->QueryIntAttribute("hp", &hp) != tinyxml2::XML_SUCCESS) {
            throw std::runtime_error("XML error: spaceship.hp is missing");
        }
        int speed = 0;
        if (elem->QueryIntAttribute("speed", &speed) != tinyxml2::XML_SUCCESS) {
            throw std::runtime_error("XML error: spaceship.speed is missing");
        }
        int points = 0;
        elem->QueryIntAttribute("points", &points);

        // Create spaceship instance
        Spaceship ship(getAnimation(animation), hp, speed);
        ship.setPoints(points);
        ship.setMovementPattern(parse_movement_pattern(elem));
        ship.setAttackPattern(parse_attack_pattern(elem));

        // Parse weapon tag
        tinyxml2::XMLElement* weapon = elem->FirstChildElement("weapon");
        if (weapon != nullptr) {
            int wx, wy;
            const char* weapon_id = weapon->Attribute("id");
            if (weapon_id == nullptr) {
                throw std::runtime_error("XML error: spaceship.weapon.id is missing");
            }
            if (weapon->QueryIntAttribute("x", &wx) != tinyxml2::XML_SUCCESS) {
                throw std::runtime_error("XML error: spaceship.weapon.x is missing");
            }
            if (weapon->QueryIntAttribute("y", &wy) != tinyxml2::XML_SUCCESS) {
                throw std::runtime_error("XML error: spaceship.weapon.y is missing");
            }
            ship.getWeapon().init(weapon_id);
            ship.getWeapon().setPosition(wx, wy);
        }

        // Parse engine tag
        tinyxml2::XMLElement* engine = elem->FirstChildElement("engine");
        if (engine != nullptr) {
            sf::Vector2f engine_offset;
            engine->QueryFloatAttribute("x", &engine_offset.x);
            engine->QueryFloatAttribute("y", &engine_offset.y);
            ship.enableEngineEffect(engine_offset);
        }

        // Insert spaceship in Spaceship map
        m_spaceships.emplace(colorId, ship);
        std::cout << "[Factory] Registered entity " << std::hex << colorId << std::endl;

        elem = elem->NextSiblingElement("spaceship");
    }
}


Spaceship* Factory::createSpaceship(int colorId) const
{
    SpaceshipMap::const_iterator it = m_spaceships.find(colorId);
    if (it == m_spaceships.end()) {
        // std::cerr << "Cannot create spaceship with colorId '" << std::hex << colorId << "'" << std::endl;
        return nullptr;
    }
    return it->second.clone();
}


void Factory::loadItemsFromXml(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0) {
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


bool Factory::hasItem(Item::Type type, int level) const
{
    ItemMap::const_iterator it = m_items.find(ItemID(type, level));
    return it != m_items.end();
}


const Item& Factory::getItem(Item::Type type, int level) const
{
    ItemMap::const_iterator it = m_items.find(ItemID(type, level));
    if (it == m_items.end()) {
        throw std::runtime_error("Item <type:" + std::string(Item::typeToString(type)) + ", lvl:" + std::to_string(level) + "> not found");
    }

    return it->second;
}


void Factory::parseItems(tinyxml2::XMLElement* elem, const char* tagname, Item::Type type)
{
    elem = elem->FirstChildElement(tagname);
    if (elem != NULL) {
        elem = elem->FirstChildElement();
        while (elem != NULL) {
            Item item(type);
            item.loadFromXmlNode(elem);
            // Store item, indexed Type + Level
            ItemID id(type, item.getLevel());
            m_items.insert(std::pair<ItemID, Item>(id, item));
            elem = elem->NextSiblingElement();
        }
    } else {
        std::cerr << "XML error: tag '" << tagname << "' not found" << std::endl;
    }
}
