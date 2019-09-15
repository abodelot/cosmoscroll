#include "Layout.hpp"

using namespace gui;


Layout::Layout(float x, float y)
{
    setOffset(x, y);
    setSpacing(10, 10);
}


void Layout::setOffset(float x, float y)
{
    offset_.x = x;
    offset_.y = y;
}


const sf::Vector2f& Layout::getOffset() const
{
    return offset_;
}


void Layout::setSpacing(float x, float y)
{
    spacing_.x = x;
    spacing_.y = y;
}


const sf::Vector2f& Layout::getSpacing() const
{
    return spacing_;
}
