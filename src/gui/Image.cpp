#include "Image.hpp"
#include "Menu.hpp"

using namespace gui;

Image::Image(Menu* parent, const sf::Image& img, float x, float y):
	Widget(parent, false)
{
	image_.SetImage(img);
	SetRect(x, y, x + image_.GetSize().x, y + image_.GetSize().y);
}


void Image::Render(sf::RenderTarget& target) const
{
	target.Draw(image_);
}
