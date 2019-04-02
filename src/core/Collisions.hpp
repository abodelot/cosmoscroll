#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <map>
#include <SFML/Graphics.hpp>

/**
 * Collision detection helper
 */
class Collisions
{
public:
    /**
     * Register a texture before performing pixel-perfect tests
     */
    static void registerTexture(const sf::Texture* texture);

    /**
     * Pixel-perfect collision
     * Supports position, origin, and texture rect modifications
     * @return a colliding with b
     */
    static bool pixelPerfectTest(const sf::Sprite& a, const sf::Sprite& b);

private:
    typedef std::map<const sf::Texture*, sf::Image> ImageMap;

    static ImageMap images_;
};

#endif // COLLISIONS_HPP
