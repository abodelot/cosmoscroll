#include "Button.hpp"
#include "Menu.hpp"
#include "utils/SFML_Helper.hpp"

using namespace gui;


Button::Button(Menu* owner, const sf::String& text, int w, int h) :
	Widget(owner, true)
{
	text_.setString(text);

	// calcul du coin inférieur droit
	int width = w == -1 ? sfh::width(text_) : w;
	int height = h == -1 ? sfh::height(text_) : h;

	Resize(width, height);
	SetAlign(Align::CENTER);

	const WidgetStyle& style = owner->GetWidgetStyle();
	text_.setCharacterSize(style.global_text_size);
	text_.setFont(*style.global_font);
	text_.setColor(style.button_text_color);
	OnStateChanged(GetState());
}


void Button::setString(const sf::String& text)
{
	text_.setString(text);
	SetAlign(align_);
}


void Button::SetTextPadding(int x, int y)
{
	text_.setPosition(x, y);
}


void Button::SetAlign(Align::EAlign align)
{
	float x = 0;
	switch (align)
	{
		case Align::LEFT:
			x = 0;
			break;
		case Align::RIGHT:
			x = GetWidth() - sfh::width(text_);
			break;
		case Align::CENTER:
			x = (GetWidth() - sfh::width(text_)) / 2;
			break;
	}
	text_.setPosition((int) x, (int) (GetHeight() - sfh::height(text_)) / 2);
	align_ = align;
}


void Button::OnKeyPressed(sf::Keyboard::Key code)
{
	if (code == sf::Keyboard::Return)
	{
		CallTheCallback();
	}
}


void Button::OnMouseClicked(int, int)
{
	CallTheCallback();
}


void Button::OnStateChanged(State::EState state)
{
	switch (state)
	{
		case State::DEFAULT:
			text_.setColor(GetOwner()->GetWidgetStyle().button_text_color);
			break;
		case State::FOCUSED:
			text_.setColor(GetOwner()->GetWidgetStyle().button_text_color_focus);
			break;
		default:
			break;
	}
}


void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(text_, states);
}
