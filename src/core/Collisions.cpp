#include "Collisions.hpp"


static inline sf::Uint8 getAlphaAt(const sf::Uint8* pixels, int imageWidth, int x, int y)
{
    // Gets to the alpha component of pixels[x, y] (Picture width being provided)
    return pixels[(x + y * imageWidth) * 4 + 3];
}


bool Collisions::pixelPerfectTest(const sf::Sprite& a, const sf::Sprite& b) const
{
    sf::IntRect rectA = a.getTextureRect();
    rectA.left = a.getPosition().x - a.getOrigin().x;
    rectA.top = a.getPosition().y - a.getOrigin().y;

    sf::IntRect rectB = b.getTextureRect();
    rectB.left = b.getPosition().x - b.getOrigin().x;
    rectB.top = b.getPosition().y - b.getOrigin().y;

    sf::IntRect overlap;

    // If overlapping rectangles
    if (rectA.intersects(rectB, overlap)) {
        if (a.getTexture() == nullptr || b.getTexture() == nullptr) {
            return true;
        }

        const sf::Image& imgA = m_images.at(a.getTexture());
        const sf::Image& imgB = m_images.at(b.getTexture());

        const int ALPHA = 0;

        const int left1 = overlap.left - rectA.left + a.getTextureRect().left;
        const int top1 = overlap.top - rectA.top + a.getTextureRect().top;
        const int left2 = overlap.left - rectB.left + b.getTextureRect().left;
        const int top2 = overlap.top - rectB.top + b.getTextureRect().top;

        const sf::Uint8* pixelsA = imgA.getPixelsPtr();
        const sf::Uint8* pixelsB = imgB.getPixelsPtr();
        unsigned int widthA = imgA.getSize().x;
        unsigned int widthB = imgB.getSize().x;

        for (int y = 0; y < overlap.height; ++y) {
            for (int x = 0; x < overlap.width; ++x) {
                if (getAlphaAt(pixelsA, widthA, x + left1, y + top1) > ALPHA &&
                    getAlphaAt(pixelsB, widthB, x + left2, y + top2) > ALPHA) {
                    return true;
                }
            }
        }
    }
    return false;
}


void Collisions::registerTexture(const sf::Texture* texture)
{
    ImageMap::const_iterator it = m_images.find(texture);
    if (it == m_images.end()) {
        m_images[texture] = texture->copyToImage();
    }
}
