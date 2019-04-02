#include "VBoxLayout.hpp"
#include "../Widget.hpp"

using namespace gui;


VBoxLayout::VBoxLayout(float x, float y): Layout(x, y)
{
}


Widget* VBoxLayout::Add(Widget* widget)
{
    sf::Vector2f pos = GetOffset();
    widget->setPosition(pos);
    // compute next widget position
    pos.y += widget->GetHeight() + GetSpacing().y;
    SetOffset(pos.x, pos.y);
    return widget;
}
