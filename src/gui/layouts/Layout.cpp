#include "Layout.hpp"

using namespace gui;


Layout::Layout(float x, float y)
{
	SetOffset(x, y);
	SetSpacing(10, 10);
}


void Layout::SetOffset(float x, float y)
{
	offset_.x = x;
	offset_.y = y;
}


const sf::Vector2f& Layout::GetOffset() const
{
	return offset_;
}


void Layout::SetSpacing(float x, float y)
{
	spacing_.x = x;
	spacing_.y = y;
}


const sf::Vector2f& Layout::GetSpacing() const
{
	return spacing_;
}
