#ifndef MATH_HPP
#define MATH_HPP

#include <cmath>
#include <cstdlib>

namespace math {

// Trigonometry ----------------------------------------------------------------

static constexpr float PI = 3.14159265f;

/**
 * Convert degrees to radians
 */
inline float to_radians(float degrees)
{
    return degrees * PI / 180;
}

/**
 * Convert radians to degrees
 */
inline float to_degrees(float radians)
{
    // Normalize between 0 and 360
    float degrees = int(radians / PI * 180) % 360;
    return degrees < 0 ? degrees + 360 : degrees;
}

/**
 * Apply a translation on a point
 * @param angle: angle in radians
 */
template <class Vec>
inline void translate(Vec& point, float angle, float distance)
{
    // Flip on the Y-axis: (0, 0) is upper left
    point.x = point.x + distance * std::cos(angle);
    point.y = point.y - distance * std::sin(angle);
}

/**
 * Distance between 2 points
 */
template <class Vec>
inline float distance(const Vec& a, const Vec& b = Vec())
{
    // Sqrt((x1 - x2)² + (y1 - y2)²)
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

/**
 * Angle between the difference vector (connecting b and a) and the X-axis
 */
template <class Vec>
inline float angle(const Vec& a, const Vec& b = Vec())
{
    // Flip on the Y-axis: (0, 0) is upper left
    return -std::atan2(b.y - a.y, b.x - a.x);
}

// Random ----------------------------------------------------------------------

/**
 * Random generator seed, initialized to current timestamp
 */
extern unsigned int g_seed;

/**
 * Get a random int value between begin and end
 */
int rand(int begin, int end);

/**
 * Get a random float value between begin and end
 */
float rand(float begin, float end);

/**
 * Initialize random number generator seed
 */
void set_seed(unsigned int seed);

// Misc ------------------------------------------------------------------------

/**
 * Ensure value is between min and max
 */
template <class T>
inline T clamp(T value, T min, T max)
{
    return value < min ? min : (value > max ? max : value);
}

}

#endif // MATH_HPP
