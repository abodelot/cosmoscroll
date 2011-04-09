#include <cstdio>
#include "BitmapFont.hpp"

using namespace gui;


BitmapFont::BitmapFont(const char* imagepath, int width, int height)
{
	width_ = width;
	height_ = height;

	SetImage(imagepath);
}


bool BitmapFont::SetImage(const char* imagepath)
{
	if (image_.LoadFromFile(imagepath))
	{
		image_.SetSmooth(false);
		char_width_ = image_.GetWidth() / width_;
		char_height_ = image_.GetHeight() / height_;
		return true;
	}
	return false;
}


const sf::Image& BitmapFont::GetImage() const
{
	return image_;
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
	subrect.Left = (character % width_) * char_width_;
	subrect.Right = subrect.Left + char_width_;
	subrect.Top = (character / width_) * char_height_;
	subrect.Bottom = subrect.Top + char_height_;
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

