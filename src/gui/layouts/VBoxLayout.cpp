#include "VBoxLayout.hpp"
#include "../Widget.hpp"

using namespace gui;


VBoxLayout::VBoxLayout(float x, float y): Layout(x, y)
{
}


Widget* VBoxLayout::add(Widget* widget)
{
    sf::Vector2f pos = getOffset();
    widget->setPosition(pos);
    // compute next widget position
    pos.y += widget->getHeight() + getSpacing().y;
    setOffset(pos.x, pos.y);
    return widget;
}
