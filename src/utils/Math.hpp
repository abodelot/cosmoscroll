#ifndef MATH_HPP
#define MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>

#define PI 3.14159265f

namespace math
{

inline float quick_bound(float x)
{
	if (x < - PI)
		x += 2 * PI;
	else if (x > PI)
		x -= 2 * PI;

	return x;
}

inline float sin(float x)
{
	/*x = quick_bound(x);
	float Sin = 1.27323954 * x;
	if (x < 0)
	{
		Sin += .405284735 * x * x;
	}
	else
	{
		Sin -= .405284735 * x * x;
	}
	x = (Sin * Sin);
	if (Sin >= 0) x = -x;

	return Sin + .255 * (x - Sin);*/
	return std::sin(x);
}


inline float cos(float x)
{
	//return math::sin(x + PI / 2);
	return std::cos(x);
}


inline float deg_to_rad(float degres)
{
	return degres * PI / 180;
}


inline float rad_to_deg(float radians)
{
	return radians / PI * 180;
}


inline void translate(sf::Vector2f& offset, float angle, float speed)
{
	offset.x = offset.x + speed * math::cos(angle);
	offset.y = offset.y - speed * math::sin(angle);
}


inline float distance(float x1, float y1, float x2 = 0.f, float y2 = 0.f)
{
	// Formule de calcul de la distance entre deux points :
	// sqrt( (x1 - x2)² + (y1 - y2)² )
	return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}


inline float distance(const sf::Vector2f& p1, const sf::Vector2f& p2 = sf::Vector2f(0.f, 0.f))
{
	return distance(p1.x, p1.y, p2.x, p2.y);
}


inline float angle(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
	float x = p1.x - p2.x;
	if (x == 0.f)
	{
		return 0.f;
	}
	float y = p1.y - p2.y;
	float radians = atan(-y / x);
	if (p2.x > p1.x)
	{
		radians += PI;
	}
	return radians;
}

}

#endif // MATH_HPP

