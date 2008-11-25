#ifndef H_MATH
#define H_MATH

#include <cmath>

#define PI 3.14159f


inline float DEG_TO_RAD(float degres)
{
    return degres * PI / 180;
}


inline float RAD_TO_DEG(float radians)
{
    return radians / PI * 180;
}


inline void TRANSLATE(sf::Vector2f& offset, float angle, float distance)
{
    offset.x = offset.x + distance * std::cos(angle);
    offset.y = offset.y - distance * std::sin(angle);
}


inline float Distance(const float x1, const float y1, const float x2 = 0.f, const float y2 = 0.f)
{
    // Formule de calcul de la distance entre deux points :
    // sqrt( (x1 - x2)² + (y1 - y2)² )
 
    return sqrt( (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) );
}
 

inline float Distance(const sf::Vector2f& p1, const sf::Vector2f& p2 = sf::Vector2f(0.f, 0.f))
{
    // Utilisation de l'autre fonction Distance.
    return Distance(p1.x, p1.y, p2.x, p2.y);
}

inline float ANGLE(const sf::Vector2f& p1, const sf::Vector2f& p2)
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

#endif /* guard H_MATH */

