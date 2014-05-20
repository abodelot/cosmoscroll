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


// Randomizer ------------------------------------------------------------------

extern unsigned int seed;
void set_seed(unsigned int seed);

inline int random(int begin, int end)
{
	return std::rand() % (end - begin + 1) + begin;
}


inline float random(float begin, float end)
{
	return static_cast<float>(std::rand()) / RAND_MAX * (end - begin) + begin;
}


// Color -----------------------------------------------------------------------

/**
 * Create color from hexadecimal code (#ffffff or #fff)
 */
sf::Color hexa_to_color(const std::string& hexcolor);

/**
 * Randomly pick a color
 */
sf::Color random_color(sf::Uint8 min_r=0,   sf::Uint8 min_g=0,   sf::Uint8 min_b=0,
                       sf::Uint8 max_r=255, sf::Uint8 max_g=255, sf::Uint8 max_b=255);

}

#endif // SFML_HELPER_HPP
