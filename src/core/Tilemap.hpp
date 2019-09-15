#ifndef TILEMAP_HPP
#define TILEMAP_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Tileset;
class EntityManager;

class Tilemap: public sf::Drawable, public sf::Transformable {
public:
    Tilemap();

    /**
     * Load tilemap from a level encoded in an image
     */
    bool loadFromImage(const Tileset& tileset, const std::string& path, EntityManager& entityManager);

    bool collides(const sf::FloatRect& rect, sf::Sprite& sprite) const;

    bool hasTile(const sf::Vector2i& coords) const;

    sf::Vector2i convertPixelsToCoords(const sf::Vector2f& point) const;

    sf::Vector2f convertCoordsToPixels(int i, int j) const;

    /**
     * Get a sprite for the tile at given coords
     */
    sf::Sprite getSpriteAt(const sf::Vector2i& coords) const;

    /**
     * Get size of tilemap area
     */
    sf::Vector2i getPixelSize() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    bool hasBlock(const char* blocks, int i, int j) const;

    int getPixel(const sf::Uint8*, int x, int y) const;

    int m_width;
    int m_height;
    sf::VertexArray m_vertices;
    const Tileset* m_tileset;
    std::vector<int> m_tiles;
};

#endif
