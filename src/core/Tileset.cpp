#include "Tileset.hpp"


enum ID {
    TOP_LEFT = 0,
    TOP = 1,
    TOP_RIGHT = 2,
    LEFT = 8,
    CENTER = 9,
    RIGHT = 10,
    BOTTOM_LEFT = 16,
    BOTTOM = 17,
    BOTTOM_RIGHT = 18,
    END_LEFT = 24,
    HORIZONTAL = 25,
    END_RIGHT = 26,
    END_TOP = 3,
    VERTICAL = 11,
    END_BOTTOM = 19,
    ALONE = 27,

    METAL_LEFT = 76,
    METAL_CENTER = 77,
    METAL_RIGHT =  78
};


bool Tileset::loadTexture(const std::string& filename)
{
    return m_texture.loadFromFile(filename);
}


int Tileset::getTileId(bool top, bool bottom, bool left, bool right) const
{
    int flag = top << 3 | bottom << 2 | left << 1 | right;
    switch (flag) {
        // top|bottom|left|right
        case 0b0000: return ALONE;
        case 0b0001: return END_LEFT;
        case 0b0010: return END_RIGHT;
        case 0b0011: return HORIZONTAL;
        case 0b0100: return END_TOP;
        case 0b0101: return TOP_LEFT;
        case 0b0110: return TOP_RIGHT;
        case 0b0111: return TOP;
        case 0b1000: return END_BOTTOM;
        case 0b1001: return BOTTOM_LEFT;
        case 0b1010: return BOTTOM_RIGHT;
        case 0b1011: return BOTTOM;
        case 0b1100: return VERTICAL;
        case 0b1101: return LEFT;
        case 0b1110: return RIGHT;
        case 0b1111: return CENTER;
    }
    return -1;
}


int Tileset::getTileId(bool left, bool right) const
{
    if (left && right) {
        return METAL_CENTER;
    }
    if (left) {
        return METAL_RIGHT;
    }
    if (right) {
        return METAL_LEFT;
    }
    return -1;
}


const sf::Texture& Tileset::getTexture() const
{
    return m_texture;
}

void Tileset::setTileSize(const sf::Vector2u& size)
{
    m_tileSize = size;
}

const sf::Vector2u& Tileset::getTileSize() const
{
    return m_tileSize;
}
