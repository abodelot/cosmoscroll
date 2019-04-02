#include "Collisions.hpp"

Collisions::ImageMap Collisions::images_;


// Gets to the alpha component of pixelsPtr[x,y] (Picture width being provided)
#define ALPHACOMP(buf, width, x, y) (buf[((x) + (y) * (width)) * 4 + 3])

bool Collisions::pixelPerfectTest(const sf::Sprite& a, const sf::Sprite& b)
{
    sf::IntRect rect_a = a.getTextureRect();
    rect_a.left = a.getPosition().x - a.getOrigin().x;
    rect_a.top =  a.getPosition().y - a.getOrigin().y;

    sf::IntRect rect_b = b.getTextureRect();
    rect_b.left = b.getPosition().x - b.getOrigin().x;
    rect_b.top =  b.getPosition().y - b.getOrigin().y;

    sf::IntRect overlap;

    // If overlapping rectangles
    if (rect_a.intersects(rect_b, overlap))
    {
        if (a.getTexture() == NULL || b.getTexture() == NULL)
            return true;

        const sf::Image& img_a = images_[a.getTexture()];
        const sf::Image& img_b = images_[b.getTexture()];

        const int ALPHA = 0;

        int left1 = overlap.left - rect_a.left + a.getTextureRect().left;
        int top1 =  overlap.top  - rect_a.top  + a.getTextureRect().top;

        int left2 = overlap.left - rect_b.left + b.getTextureRect().left;
        int top2 =  overlap.top  - rect_b.top  + b.getTextureRect().top;

        const sf::Uint8 *pix_a = img_a.getPixelsPtr();
        const sf::Uint8 *pix_b = img_b.getPixelsPtr();

        int width_a = img_a.getSize().x;
        int width_b = img_b.getSize().x;

        for (int y = 0; y < overlap.height; ++y)
        {
            for (int x = 0; x < overlap.width; ++x)
            {
                if (ALPHACOMP(pix_a, width_a, (x + left1), (y + top1)) > ALPHA &&
                    ALPHACOMP(pix_b, width_b, (x + left2), (y + top2)) > ALPHA)
                {
                    return true;
                }
            }
        }
    }
    return false;
}


void Collisions::registerTexture(const sf::Texture* texture)
{
    ImageMap::const_iterator it = images_.find(texture);
    if (it == images_.end())
        images_[texture] = texture->copyToImage();
}
