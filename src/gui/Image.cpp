#include "Image.hpp"
#include "Menu.hpp"

using namespace gui;

Image::Image(Menu* parent, const sf::Image& img, float x, float y):
	Widget(parent, false)
{
	image_.SetImage(img);
	Resize(image_.GetSize().x, image_.GetSize().y);
}


void Image::Render(sf::RenderTarget& target) const
{
	target.Draw(image_);
}
