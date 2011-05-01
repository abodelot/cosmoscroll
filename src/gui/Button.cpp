#include "Button.hpp"
#include "Menu.hpp"

#define PADDING_Y 8
#define PADDING_X 0

using namespace gui;


Button::Button(Menu* owner, const sf::Unicode::Text& text, int x, int y, int w, int h) :
	Widget(owner, true)
{
	text_.SetText(text);
	text_.SetPosition(PADDING_X, PADDING_Y);

	// calcul du coin inférieur droit
	int width = w == -1 ? text_.GetRect().GetWidth() + PADDING_X * 2 : w;
	int height = h == -1 ? text_.GetRect().GetHeight() + PADDING_Y * 2 : h;

	SetRect(x, y, x + width, y + height);
	SetAlign(Align::CENTER);

	WidgetStyle& style = owner->GetWidgetStyle();
	text_.SetSize(style.global_text_size);
	text_.SetFont(*style.global_font);
	text_.SetColor(style.button_text_color);
	OnStateChanged(GetState());
}


void Button::SetText(const sf::Unicode::Text& text)
{
	text_.SetText(text);
}


void Button::SetAlign(Align::EAlign align)
{
	switch (align)
	{
		case Align::LEFT:
			text_.SetX(PADDING_X);
			break;
		case Align::RIGHT:
			text_.SetX(GetWidth() - text_.GetRect().GetWidth());
			break;
		case Align::CENTER:
			text_.SetX((GetWidth() - text_.GetRect().GetWidth()) / 2);
			break;
	}
}


void Button::OnKeyPressed(sf::Key::Code code)
{
	if (code == sf::Key::Return)
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
			text_.SetColor(GetOwner()->GetWidgetStyle().button_text_color);
			break;
		case State::FOCUSED:
			text_.SetColor(GetOwner()->GetWidgetStyle().button_text_color_focus);
			break;
		default:
			break;
	}
}


void Button::Render(sf::RenderTarget& target) const
{
	target.Draw(text_);
}
