#ifndef SFML_HELPER_HPP
#define SFML_HELPER_HPP

#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <iostream>

namespace sfh
{

// Transformable ---------------------------------------------------------------

template <class T>
inline void setX(T& obj, float x)
{
    obj.setPosition(x, obj.getPosition().y);
}

template <class T>
inline void setY(T& obj, float y)
{
    obj.setPosition(obj.getPosition().x, y);
}

template <class T>
float width(const T& obj)
{
    return obj.getLocalBounds().width * obj.getScale().x;
}

template <class T>
inline float height(const T& obj)
{
    return obj.getLocalBounds().height * obj.getScale().y;
}

sf::Vector2f getSize(const sf::Sprite& sprite);

void resize(sf::Sprite& sprite, float width, float height);

sf::Vector2f getCenter(const sf::Sprite& sprite);

inline bool disjoint(const sf::FloatRect& a, const sf::FloatRect& b)
{
    return a.top > (b.top + b.height) ||
        a.left > (b.left + b.width) ||
        (a.top + a.height) < b.top ||
        (a.left + a.width) < b.left;
}

// Color -----------------------------------------------------------------------

/**
 * Create color from hexadecimal code (#ffffff or #fff)
 */
sf::Color hexa_to_color(const std::string& hexcolor);

}

std::ostream& operator<<(std::ostream& os, const sf::FloatRect& rect);
std::ostream& operator<<(std::ostream& os, const sf::Vector2f& pos);

#endif // SFML_HELPER_HPP
