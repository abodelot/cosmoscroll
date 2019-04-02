#ifndef SFML_HELPER_HPP
#define SFML_HELPER_HPP

#include <cstdlib>
#include <SFML/Graphics.hpp>

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

template <class T>
float right(const T& obj)
{
    return obj.getPosition().x + width(obj);
}

template <class T>
inline float bottom(const T& obj)
{
    return obj.getPosition().y + height(obj);
}


sf::Vector2f size(const sf::Sprite& sprite);
void resize(sf::Sprite& sprite, float width, float height);
sf::Vector2f getCenter(const sf::Sprite& sprite);

sf::Vector2f size(const sf::Text& text);
void resize(sf::Text& text, float width, float height);
sf::Vector2f getCenter(const sf::Text& text);

// Color -----------------------------------------------------------------------

/**
 * Create color from hexadecimal code (#ffffff or #fff)
 */
sf::Color hexa_to_color(const std::string& hexcolor);

}

#endif // SFML_HELPER_HPP
