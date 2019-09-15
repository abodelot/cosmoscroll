#include "Widget.hpp"
#include "Menu.hpp"

using namespace gui;

Widget::Widget(Menu* owner, bool focusable):
    owner_(owner),
    width_(0),
    height_(0),
    focusable_(focusable),
    visible_(true),
    m_state(State::DEFAULT)
{
    owner->addWidget(this);
}


void Widget::setState(State::EState state)
{
    onStateChanged(state);
    m_state = state;
}


State::EState Widget::getState() const
{
    return m_state;
}


void Widget::update(float)
{
}


bool Widget::containsPoint(float x, float y)
{
    return (x >= getPosition().x) &&
        (x <= getPosition().x + width_) &&
        (y >= getPosition().y) &&
        (y <= getPosition().y + height_);
}


void Widget::resize(int width, int height)
{
    width_ = width;
    height_ = height;
}


int Widget::getWidth() const
{
    return width_;
}


int Widget::getHeight() const
{
    return height_;
}


bool Widget::canGrabFocus() const
{
    return focusable_ && visible_ && m_state != State::DISABLED;
}


bool Widget::isFocused() const
{
    return this == getOwner()->getFocusedWidget();
}


Menu* Widget::getOwner() const
{
    return owner_;
}


void Widget::setCallback(std::function<void(void)> callback)
{
    callback_ = callback;
}


void Widget::triggerCallback()
{
    if (callback_)
    {
        callback_();
    }
}


void Widget::centerText(sf::Text& text)
{
    float textHeight = text.getFont()->getLineSpacing(text.getCharacterSize());
    float textWidth = text.getLocalBounds().width;
    // Cast position to int for pixel-perfect rendering
    text.setPosition(
        (int) (width_ - textWidth) / 2,
        (int) (height_ - textHeight) / 2
    );
}
