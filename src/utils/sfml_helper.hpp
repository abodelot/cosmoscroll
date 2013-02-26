#ifndef XSFML_HPP
#define XSFML_HPP


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
		sf::Vector2f center;
		center.x = getPosition().x + getTextureRect().width  / 2;
		center.y = getPosition().y + getTextureRect().height / 2;
		return center;
	}

};

class Text: public sf::Text
{
public:
	Text(): sf::Text() {}
	Text(const sf::String& string) { setString(string); }

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
		sf::Vector2f center;
		center.x = getPosition().x + getLocalBounds().width  / 2;
		center.y = getPosition().y + getLocalBounds().height / 2;
		return center;
	}
};




// color -----------------------------------------------------------------------

// create color from hexadecimal code (#ffffff or #fff)
inline sf::Color hexa_to_color(const std::string& hexcolor)
{
	sf::Color color = sf::Color::Black;
	if (hexcolor.size() == 7) // #ffffff
	{
		color.r = strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16);
		color.g = strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16);
		color.b = strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16);
	}
	else if (hexcolor.size() == 4) // #fff
	{
		color.r = strtoul(hexcolor.substr(1, 1).c_str(), NULL, 16) * 17;
		color.g = strtoul(hexcolor.substr(2, 1).c_str(), NULL, 16) * 17;
		color.b = strtoul(hexcolor.substr(3, 1).c_str(), NULL, 16) * 17;
	}
	return color;
}


}

#endif // XSFML_HPP






