#include <cstdio>
#include "BitmapFont.hpp"

using namespace gui;


BitmapFont::BitmapFont(const sf::Texture& image, int width, int height)
{
	width_ = width;
	height_ = height;

	setTexture(image);
}


bool BitmapFont::setTexture(const sf::Texture& image)
{
	m_texture = &image;
	char_width_ = image.getSize().x / width_;
	char_height_ = image.getSize().y / height_;
	return true;
}


const sf::Texture& BitmapFont::getTexture() const
{
	return *m_texture;
}


sf::IntRect BitmapFont::GetCharRect(char character) const
{
	if (character < FIRST_CHAR || character > LAST_CHAR)
	{
		printf("warning: caractère non imprimable (ASCII %d)\n", character);
		character = '?';
	}
	character -= FIRST_CHAR;

	sf::IntRect subrect;
	subrect.left = (character % width_) * char_width_;
	subrect.width = char_width_;
	subrect.top = (character / width_) * char_height_;
	subrect.height = char_height_;
	return subrect;
}


int BitmapFont::GetCharWidth() const
{
	return char_width_;
}


int BitmapFont::GetCharHeight() const
{
	return char_height_;
}

