#include "Math.hpp"
#include <SFML/System/Randomizer.hpp>

namespace math
{

float angle(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float x = p1.x - p2.x;
	if (x == 0.f)
	{
		return 0.f;
	}
	float y = p1.y - p2.y;
	float radians = std::atan(-y / x);
	if (p2.x > p1.x)
	{
		radians += PI;
	}
	return radians;
}


sf::Color random_color(sf::Uint8 min_r, sf::Uint8 min_g, sf::Uint8 min_b, sf::Uint8 max_r, sf::Uint8 max_g, sf::Uint8 max_b)
{
	return sf::Color(
		sf::Randomizer::Random(min_r, max_r),
		sf::Randomizer::Random(min_g, max_g),
		sf::Randomizer::Random(min_b, max_b)
	);
}

}
