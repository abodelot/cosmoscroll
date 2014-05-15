#ifndef XSFML_HPP
#define XSFML_HPP

#include <cstdlib>
#include <SFML/Graphics.hpp>

namespace sfh
{

sf::Vector2f size(const sf::Sprite& sprite);

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

}


namespace xsf
{

class Sprite: public sf::Sprite
{
public:
	inline void setX(float x) { setPosition(x, getPosition().y); }
	inline void setY(float y) { setPosition(getPosition().x, y); }

	inline float getX() const { return getPosition().x; }
	inline float getY() const { return getPosition().y; }

	inline float getWidth() const  { return getTextureRect().width  * getScale().x; }
	inline float getHeight() const { return getTextureRect().height * getScale().y; }

	void resize(float width, float height)
	{
		int LocalWidth  = getTextureRect().width;
		int LocalHeight = getTextureRect().height;

		if ((LocalWidth > 0) && (LocalHeight > 0))
			setScale(width / LocalWidth, height / LocalHeight);
	}

	// return center of sprite
	inline sf::Vector2f getCenter() const
	{
		sf::Vector2f center = getPosition();
		center.x += getTextureRect().width  / 2.f;
		center.y += getTextureRect().height / 2.f;
		return center;
	}
};

class Text: public sf::Text
{
public:
	inline void setX(float x) { setPosition(x, getPosition().y); }
	inline void setY(float y) { setPosition(getPosition().x, y); }

	inline float getX() const { return getPosition().x; }
	inline float getY() const { return getPosition().y; }

	inline float getWidth() const  { return getLocalBounds().width  * getScale().x; }
	inline float getHeight() const { return getLocalBounds().height * getScale().y; }

	void resize(float width, float height)
	{
		int LocalWidth  = getLocalBounds().width;
		int LocalHeight = getLocalBounds().height;

		if ((LocalWidth > 0) && (LocalHeight > 0))
			setScale(width / LocalWidth, height / LocalHeight);
	}

	// return center of text
	inline sf::Vector2f getCenter() const
	{
		sf::Vector2f center = getPosition();
		center.x += getLocalBounds().width  / 2.f;
		center.y += getLocalBounds().height / 2.f;
		return center;
	}
};


// random ----------------------------------------------------------------------

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


// color -----------------------------------------------------------------------

/**
 * Create color from hexadecimal code (#ffffff or #fff)
 */
sf::Color hexa_to_color(const std::string& hexcolor);

/**
 * Randomly pick a color
 */
sf::Color random_color(sf::Uint8 min_r=0, sf::Uint8 min_g=0, sf::Uint8 min_b=0, sf::Uint8 max_r=255, sf::Uint8 max_g=255, sf::Uint8 max_b=255);

}

#endif // XSFML_HPP
