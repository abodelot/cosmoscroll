#ifndef COLLISIONS_HPP
#define COLLISIONS_HPP

#include <SFML/Graphics.hpp>
#include <unordered_map>

/**
 * Collision detection helper
 */
class Collisions {
public:
    /**
     * Register a texture before performing pixel-perfect tests
     */
    void registerTexture(const sf::Texture* texture);

    /**
     * Pixel-perfect collision
     * Supports position, origin, and texture rect modifications
     * @return a colliding with b
     */
    bool pixelPerfectTest(const sf::Sprite& a, const sf::Sprite& b) const;

private:
    typedef std::unordered_map<const sf::Texture*, sf::Image> ImageMap;

    ImageMap m_images;
};

#endif
