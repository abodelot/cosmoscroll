#include "Slider.hpp"
#include "Menu.hpp"

using namespace gui;


Slider::Slider(Menu* owner, int x, int y, int w, int h) :
	Widget(owner, true)
{
	if (h == -1)
	{
		h = owner->GetWidgetStyle().global_text_size;
	}
	bar_ = sf::Shape::Rectangle(0, 0, w, h, sf::Color::White,
		1, owner->GetWidgetStyle().global_border_color);
	handle_ = sf::Shape::Rectangle(0, 0, h, h,
		sf::Color::White, 1, owner->GetWidgetStyle().global_border_color);
	SetState(State::DEFAULT);

	handle_index_ = 0;
	quantum_ = 10;

	SetRect(x, y, x + w, y + h);
	OnStateChanged(GetState());
}


void Slider::SetQuantum(int quantum)
{
	if (quantum > 0 && quantum < 100)
	{
		quantum_ = quantum;
	}
}


int Slider::GetValue() const
{
	return handle_index_;
}


void Slider::SetValue(int value)
{
	UpdateHandle(value);
}


void Slider::OnKeyPressed(sf::Key::Code key)
{
	switch (key)
	{
		case sf::Key::Left:
			UpdateHandle(handle_index_ - quantum_);
			break;
		case sf::Key::Right:
			UpdateHandle(handle_index_ + quantum_);
			break;
		case sf::Key::Home:
			UpdateHandle(0);
			break;
		case sf::Key::End:
			UpdateHandle(100);
			break;
		default:
			break;
	}
}


void Slider::OnMouseClicked(int x, int y)
{
	(void) y;
	UpdateHandle(100 * x / GetWidth());
}


void Slider::OnMouseWheelMoved(int delta)
{
	UpdateHandle(handle_index_ + (delta > 0 ? quantum_ : -quantum_));
}


void Slider::OnStateChanged(State::EState state)
{
	const WidgetStyle& style = GetOwner()->GetWidgetStyle();
	switch (state)
	{
		case State::DEFAULT:
			bar_.SetColor(style.slider_bg_color);
			handle_.SetColor(style.slider_handle_color);
			break;
		case State::FOCUSED:
			bar_.SetColor(style.slider_bg_color_focus);
			handle_.SetColor(style.slider_handle_color_focus);
			break;
		default:
			break;
	}
}


void Slider::Render(sf::RenderTarget& target) const
{
	target.Draw(bar_);
	target.Draw(handle_);
}


void Slider::UpdateHandle(int value)
{
	if (value < 0)
	{
		value = 0;
	}
	else if (value > 100)
	{
		value = 100;
	}
	else
	{
		// round value to a quantum multiple
		int temp = value + quantum_ / 2;
		value = temp - temp % quantum_;
	}
	// check if value actually changed
	if (value != handle_index_)
	{
		handle_index_ = value;
		CallTheCallback();
	}
	// then update handle sprite (handle is a square, size is Rect.Height)
	int x = (GetRect().GetWidth() - GetRect().GetHeight()) * handle_index_ / 100;
	handle_.SetX(x);
}

