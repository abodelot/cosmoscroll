#include "Widget.hpp"
#include "Menu.hpp"

using namespace gui;

Widget::Widget(Menu* owner, bool focusable)
{
	focusable_ = focusable;
	state_ = State::DEFAULT;
	visible_ = true;
	callback_id_ = -1;
	owner_ = owner;
	owner->AddWidget(this);
}


void Widget::SetState(State::EState state)
{
	OnStateChanged(state);
	state_ = state;
}


State::EState Widget::GetState() const
{
	return state_;
}


void Widget::Update(float) {}


const sf::FloatRect& Widget::GetRect() const
{
	return rect_;
}


int Widget::GetWidth() const
{
	return rect_.GetWidth();
}


int Widget::GetHeight() const
{
	return rect_.GetHeight();
}


bool Widget::CanGrabFocus() const
{
	return focusable_ && visible_ && state_ != State::DISABLED;
}


bool Widget::IsFocused() const
{
	return this == GetOwner()->GetFocusedWidget();
}


void Widget::SetRect(int x, int y, int x2, int y2)
{
	SetRect(sf::FloatRect(x, y, x2, y2));
}


void Widget::SetRect(const sf::FloatRect& rect)
{
	rect_ = rect;
	SetPosition(rect.Left, rect.Top);
}


Menu* Widget::GetOwner() const
{
	return owner_;
}


void Widget::CallTheCallback()
{
	OnCallbackTriggered();
	if (callback_id_ != -1)
	{
		owner_->EventCallback(callback_id_);
	}
}
