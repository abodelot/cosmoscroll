#include "Image.hpp"
#include "Menu.hpp"

using namespace gui;

Image::Image(Menu* parent, const sf::Texture& img, float x, float y):
	Widget(parent, false)
{
	m_texture.setTexture(img);
	setPosition(x, y);
	Resize(img.getSize().x, img.getSize().y);
}


void Image::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_texture, states);
}
