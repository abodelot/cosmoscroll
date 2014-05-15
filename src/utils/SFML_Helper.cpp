#include <ctime>
#include "SFML_Helper.hpp"

namespace sfh
{

sf::Vector2f size(const sf::Sprite& sprite)
{
	const sf::IntRect& rect = sprite.getTextureRect();
	return sf::Vector2f(rect.width * sprite.getScale().x, rect.height * sprite.getScale().y);
}




}


namespace xsf
{

// Set the random numbers sequence seed with the current system time, so that it is always different
unsigned int static set_random_seed()
{
	unsigned int seed = static_cast<unsigned int>(time(NULL));
	srand(seed);
	return seed;
}
unsigned int seed = set_random_seed();


void set_seed(unsigned int s)
{
    srand(s);
    seed = s;
}


sf::Color hexa_to_color(const std::string& hexcolor)
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


sf::Color random_color(sf::Uint8 min_r, sf::Uint8 min_g, sf::Uint8 min_b, sf::Uint8 max_r, sf::Uint8 max_g, sf::Uint8 max_b)
{
	return sf::Color(random(min_r, max_r), random(min_g, max_g), random(min_b, max_b));
}

}
