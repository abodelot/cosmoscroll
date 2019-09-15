#include "Tilemap.hpp"
#include "Tileset.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Asteroid.hpp"
#include "entities/Spaceship.hpp"
#include "entities/decors/Gate.hpp"
#include "entities/decors/GunTower.hpp"
#include "entities/decors/Canon.hpp"
#include "entities/bosses/Boss1.hpp"
#include "entities/bosses/FlyingSaucerBoss.hpp"
#include "entities/bosses/TentaculatBoss.hpp"
#include "entities/bosses/BrainBoss.hpp"
#include "core/Services.hpp"

#include <SFML/Graphics/Vertex.hpp>
#include <cmath>
#include <iostream>

static constexpr int TILE_SIZE = 16;

static constexpr int COLOR_INDUSTRIAL_2 = 0x37446e;

enum {
    TILE_BROWN_ROCK = 0xe1ac5c,
    TILE_METAL_RODE = 0xaaaaaa,

    TILE_INDUSTRIAL = 0xddcdac,
    TILE_GREEN = 0x00c600,

    ASTEROID = 0x333333,

    // #00ff??: bosses
    BOSS_1 = 0x00ff01,
    BOSS_2 = 0x00ff02,
    BOSS_3 = 0x00ff03,
    BOSS_4 = 0x00ff04,
    BOSS_5 = 0x00ff05,
    BOSS_6 = 0x00ff06,
    BOSS_7 = 0x00ff07,
    BOSS_8 = 0x00ff08,

    // #ff00ff: decors
    DECOR_GATE = 0xff00ff,
    DECOR_GUN_TURRET = 0xff10ff,
    DECOR_CANON = 0xff20ff,
};


static constexpr int NO_TILE = -1;

static Entity* buildEntityFromColor(int color)
{
    switch (color) {
        case DECOR_GATE: return new Gate();
        case DECOR_GUN_TURRET: return new GunTower();
        case DECOR_CANON: return new Canon();
        case ASTEROID: return new Asteroid();
        case BOSS_1: return new Boss1();
        case BOSS_2: return new TentaculatBoss();
        case BOSS_3: return new FlyingSaucerBoss();
        case BOSS_4: return new BrainBoss();
    }
    return Services::getFactory().createSpaceship(color);
}


Tilemap::Tilemap():
    m_width(0),
    m_height(0),
    m_tileset(nullptr)
{
}


bool Tilemap::loadFromImage(const Tileset& tileset, const std::string& path, EntityManager& entityManager)
{
    std::cout << "[Tilemap] loading from " << path << std::endl;
    sf::Image image;
    if (!image.loadFromFile(path)) {
        std::cerr << "[Tilemap] Cannot open tilemap image: " << path << std::endl;
        return false;
    }

    m_tileset = &tileset;
    m_width = image.getSize().x;
    m_height = image.getSize().y;

    const int tileCount = m_width * m_height;
    m_tiles.clear();
    m_tiles.resize(tileCount, NO_TILE);

    m_vertices.clear();
    m_vertices.setPrimitiveType(sf::Quads);

    const sf::Vector2u& tileSize = tileset.getTileSize();
    const sf::Uint8* rgba = image.getPixelsPtr();

    // Populate the vertex array, with one quad per tile
    for (int i = 0; i < m_width; ++i) {
        for (int j = 0; j < m_height; ++j) {
            int index = i + j * m_width;
            int pixel = getPixel(rgba, i, j);

            // Empty (black or blue)
            // Blue is a special color, used for visual hint in level images, but without any effect
            if (pixel == 0 || pixel == 0x0000ff) {
                m_tiles[index] = NO_TILE;
                continue;
            }

            // Pixel => Entity
            if (Entity* entity = buildEntityFromColor(pixel)) {
                m_tiles[index] = NO_TILE;
                entity->setSpawnPosition(convertCoordsToPixels(i, j));
                entityManager.scheduleForSpawning(entity);
                continue;
            }

            // Look up neighors
            bool left = getPixel(rgba, i - 1, j) == pixel;
            bool right = getPixel(rgba, i + 1, j) == pixel;
            bool top = getPixel(rgba, i, j - 1) == pixel;
            bool bottom = getPixel(rgba, i, j + 1) == pixel;

            // Get the tile number, which is defined by the 4 surrounding tiles
            int tileNumber = 0;
            switch (pixel) {
            case TILE_BROWN_ROCK:
                tileNumber = m_tileset->getTileId(top, bottom, left, right);
                break;
            case TILE_METAL_RODE:
                tileNumber = m_tileset->getTileId(top, bottom, left, right) + 4;
                break;
            case TILE_INDUSTRIAL:
                tileNumber = m_tileset->getTileId(top, bottom, left, right) + 32;
                break;
            case COLOR_INDUSTRIAL_2:
                tileNumber = m_tileset->getTileId(top, bottom, left, right) + 64;
                break;
            case TILE_GREEN:
                tileNumber = m_tileset->getTileId(top, bottom, left, right) + 96;
                break;
            default:
                fprintf(
                    stderr,
                    "[Tilemap] Unknown pixel color %06x at {x: %d, y: %d} (skipping)\n",
                    pixel, i, j);
                continue;
            }

            m_tiles[index] = tileNumber;
            if (tileNumber >= 0) {
                // Find its position in the tileset texture
                int tu = tileNumber % (tileset.getTexture().getSize().x / tileSize.x);
                int tv = tileNumber / (tileset.getTexture().getSize().x / tileSize.x);

                // Get a pointer to the current tile's quad
                sf::Vertex quad[4];

                // Define its 4 corners
                quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
                quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
                quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
                quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

                // Define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);

                m_vertices.append(quad[0]);
                m_vertices.append(quad[1]);
                m_vertices.append(quad[2]);
                m_vertices.append(quad[3]);
            }
        }
    }

    size_t verticeSize = sizeof(sf::Vertex) * m_vertices.getVertexCount();
    std::cout << "[Tilemap] vertices = " << verticeSize / 1000.0 << " KB" << std::endl;
    return true;
}


void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture = &m_tileset->getTexture();
    target.draw(m_vertices, states);
}


sf::Vector2i Tilemap::convertPixelsToCoords(const sf::Vector2f& pos) const
{
    return sf::Vector2i(std::floor(pos.x / TILE_SIZE), std::floor(pos.y / TILE_SIZE));
}


sf::Vector2f Tilemap::convertCoordsToPixels(int i, int j) const
{
    return sf::Vector2f(i * TILE_SIZE, j * TILE_SIZE);
}


bool Tilemap::hasTile(const sf::Vector2i& coords) const
{
    if (coords.x >= 0 && coords.x < m_width && coords.y >= 0 && coords.y < m_height) {
        int tileIndex = coords.x + coords.y * m_width;
        return m_tiles.at(tileIndex) != NO_TILE;
    }
    return false;
}


sf::Sprite Tilemap::getSpriteAt(const sf::Vector2i& coords) const
{
    sf::Sprite sprite;
    if (coords.x >= 0 && coords.x < m_width && coords.y >= 0 && coords.y < m_height) {
        sprite.setTexture(m_tileset->getTexture());

        const sf::Vector2u& tileSize = m_tileset->getTileSize();
        sprite.setPosition(
           sf::Vector2f(coords.x * tileSize.x, coords.y * tileSize.y));

        int tileNumber = m_tiles.at(coords.x + coords.y * m_width);
        int tu = tileNumber % (m_tileset->getTexture().getSize().x / tileSize.x);
        int tv = tileNumber / (m_tileset->getTexture().getSize().x / tileSize.x);
        sprite.setTextureRect(
            sf::IntRect(tu * tileSize.x, tv * tileSize.y, tileSize.x, tileSize.y)
        );
    }
    return sprite;
}


bool Tilemap::hasBlock(const char* blocks, int i, int j) const
{
    if (i >= 0 && i < m_width && j >= 0 && j < m_height) {
        return blocks[i + j * m_width] != '.';
    }
    return true;
}


sf::Vector2i Tilemap::getPixelSize() const
{
    return sf::Vector2i(m_width * TILE_SIZE, m_height * TILE_SIZE);
}


int Tilemap::getPixel(const sf::Uint8* rgba, int x, int y) const
{
    // Each pixel is 4 bytes (RGB), but we only care about extracting the first 3 bytes (RGB)
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        int offset = (x + y * m_width) * 4;
        return rgba[offset] << 16 | rgba[offset + 1] << 8 | rgba[offset + 2];
    }
    return 0;
}
