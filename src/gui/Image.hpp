#ifndef GUI_IMAGE_HPP
#define GUI_IMAGE_HPP

#include "Widget.hpp"

namespace gui
{

class Image: public Widget
{
public:
    Image(Menu* parent, const sf::Texture& img, float x=0, float y=0);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    sf::Sprite m_texture;
};

}

#endif // GUI_IMAGE_HPP
