#include <ctime>
#include "SFML_Helper.hpp"

namespace sfh
{

// Transformable ---------------------------------------------------------------

sf::Vector2f getSize(const sf::Sprite& sprite)
{
    const sf::IntRect& rect = sprite.getTextureRect();
    return sf::Vector2f(rect.width * sprite.getScale().x, rect.height * sprite.getScale().y);
}


void resize(sf::Sprite& sprite, float width, float height)
{
    int local_width  = sprite.getTextureRect().width;
    int local_height = sprite.getTextureRect().height;

    if (local_width > 0 && local_height > 0) {
        sprite.setScale(width / local_width, height / local_height);
    }
}


sf::Vector2f getCenter(const sf::Sprite& sprite)
{
    sf::Vector2f center = sprite.getPosition();
    center.x += sprite.getTextureRect().width  / 2.f;
    center.y += sprite.getTextureRect().height / 2.f;
    return center;
}

// Color -----------------------------------------------------------------------

sf::Color hexa_to_color(const std::string& hexcolor)
{
    sf::Color color = sf::Color::Black;
    if (hexcolor.size() == 7) {
        // #ffffff
        color.r = strtoul(hexcolor.substr(1, 2).c_str(), nullptr, 16);
        color.g = strtoul(hexcolor.substr(3, 2).c_str(), nullptr, 16);
        color.b = strtoul(hexcolor.substr(5, 2).c_str(), nullptr, 16);
    } else if (hexcolor.size() == 4) {
        // #fff
        color.r = strtoul(hexcolor.substr(1, 1).c_str(), nullptr, 16) * 17;
        color.g = strtoul(hexcolor.substr(2, 1).c_str(), nullptr, 16) * 17;
        color.b = strtoul(hexcolor.substr(3, 1).c_str(), nullptr, 16) * 17;
    }
    return color;
}

}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& pos)
{
    os << "{x: " << pos.x << ", y: " << pos.y << "}";
    return os;
}


std::ostream& operator<<(std::ostream& os, const sf::FloatRect& rect)
{
    os << "{x: " << rect.left << ", y: " << rect.top << ", w: " << rect.width << ", h: "
       << rect.height << "}";
    return os;
}
