#include "CheckBox.hpp"
#include "Menu.hpp"

#define SIZE 20
#define PADDING 3
#define V_WIDTH 3

using namespace gui;

// TODO: gérer la taille du carré de la checkbox dans le style

CheckBox::CheckBox(Menu* owner, int x, int y) :
	Widget(owner, true)
{
	checked_ = false;
	WidgetStyle& style = owner->GetWidgetStyle();

	box_ = sf::Shape::Rectangle(0, 0, SIZE, SIZE, sf::Color::White,
		1, style.global_border_color);
	v1_ = sf::Shape::Line(PADDING, SIZE / 2, SIZE / 2, SIZE - PADDING,
		V_WIDTH, sf::Color::White);
	v2_ = sf::Shape::Line(SIZE / 2, SIZE - PADDING, SIZE - PADDING, PADDING,
		V_WIDTH, sf::Color::White);

	box_.SetColor(style.ckbox_bg_color);
	v1_.SetColor(style.ckbox_v_color);
	v2_.SetColor(style.ckbox_v_color);

	SetRect(x, y, x + SIZE, y + SIZE);
	SetState(State::DEFAULT);
}


bool CheckBox::Checked() const
{
	return checked_;
}


void CheckBox::Check(bool checked)
{
	if (checked != checked_)
	{
		checked_ = checked;
		CallTheCallback();
	}
}


void CheckBox::OnKeyPressed(sf::Key::Code code)
{
	if (code == sf::Key::Return)
	{
		checked_ = !checked_;
		CallTheCallback();
	}
}


void CheckBox::OnMouseClicked(int, int)
{
	checked_ = !checked_;
	CallTheCallback();
}


void CheckBox::OnStateChanged(State::EState state)
{
	const WidgetStyle& style = GetOwner()->GetWidgetStyle();
	switch (state)
	{
		case State::DEFAULT:
			box_.SetColor(style.ckbox_bg_color);
			v1_.SetColor(style.ckbox_v_color);
			v2_.SetColor(style.ckbox_v_color);
			break;
		case State::FOCUSED:
			box_.SetColor(style.ckbox_bg_color_focus);
			v1_.SetColor(style.ckbox_v_color_focus);
			v2_.SetColor(style.ckbox_v_color_focus);
			break;
		default:
			break;
	}
}


void CheckBox::Render(sf::RenderTarget& target) const
{
	target.Draw(box_);
	if (checked_) {
		target.Draw(v1_);
		target.Draw(v2_);
	}
}
