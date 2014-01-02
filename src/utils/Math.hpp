#ifndef MATH_HPP
#define MATH_HPP

#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace math
{

extern const float PI;

// misc helpers

template <class T>
inline T clamp(T value, T max, T min)
{
	return value < max ? max : (value > min ? value : min);
}


// 2D helpers

inline float to_rad(float degres)
{
	return degres * PI / 180;
}


inline float to_deg(float radians)
{
	return radians / PI * 180;
}


inline void translate(sf::Vector2f& offset, float angle, float speed)
{
	offset.x = offset.x + speed * std::cos(angle);
	offset.y = offset.y - speed * std::sin(angle);
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


float angle(const sf::Vector2f& p1, const sf::Vector2f& p2);

}

#endif // MATH_HPP
