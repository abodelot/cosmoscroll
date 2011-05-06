#include "Button.hpp"
#include "Menu.hpp"

using namespace gui;


Button::Button(Menu* owner, const sf::Unicode::Text& text, int x, int y, int w, int h) :
	Widget(owner, true)
{
	text_.SetText(text);

	// calcul du coin inférieur droit
	int width = w == -1 ? text_.GetRect().GetWidth() : w;
	int height = h == -1 ? text_.GetRect().GetHeight() : h;

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
	SetAlign(align_);
}


void Button::SetTextPadding(int x, int y)
{
	text_.SetPosition(x, y);
}


void Button::SetAlign(Align::EAlign align)
{
	switch (align)
	{
		case Align::LEFT:
			text_.SetX(0);
			break;
		case Align::RIGHT:
			text_.SetX(GetWidth() - text_.GetRect().GetWidth());
			break;
		case Align::CENTER:
			text_.SetX((GetWidth() - text_.GetRect().GetWidth()) / 2);
			break;
	}
	align_ = align;
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
