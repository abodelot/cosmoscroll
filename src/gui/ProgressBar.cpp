#include "ProgressBar.hpp"
#include "Menu.hpp"

#define BG_COLOR  sf::Color::White
#define BAR_COLOR sf::Color::Blue

using namespace gui;


ProgressBar::ProgressBar(Menu* owner, int w, int h) :
	Widget(owner, false)
{
	value_ = 0;
	background_ = sf::Shape::Rectangle(0, 0, w, h,
		sf::Color::White, 1, sf::Color::Black);
	background_.SetColor(BG_COLOR);

	bar_.Resize(0, h);
	bar_.SetColor(BAR_COLOR);

	Resize(w, h);
}


void ProgressBar::SetValue(int value)
{
	if (value < 0)
	{
		value = 0;
	}
	else if (value > 100)
	{
		value = 100;
	}
	value_ = value;
	float length = (float) value * GetWidth() / 100;
	bar_.Resize(length == 0 ? 0.1 : length, GetHeight());
}


int ProgressBar::GetValue() const
{
	return value_;
}


void ProgressBar::Render(sf::RenderTarget& target) const
{
	target.Draw(background_);
	target.Draw(bar_);
}
