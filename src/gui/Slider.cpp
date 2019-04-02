#include "Slider.hpp"
#include "Menu.hpp"

using namespace gui;


Slider::Slider(Menu* owner, int w, int h) :
    Widget(owner, true)
{
    if (h == -1)
    {
        h = owner->GetWidgetStyle().global_text_size;
    }
    bar_.setSize(sf::Vector2f(w, h));
    bar_.setOutlineThickness(1);
    bar_.setOutlineColor(owner->GetWidgetStyle().global_border_color);

    handle_.setSize(sf::Vector2f(h, h));
    handle_.setOutlineThickness(1);
    handle_.setOutlineColor(owner->GetWidgetStyle().global_border_color);

    SetState(State::DEFAULT);

    handle_index_ = 0;
    quantum_ = 10;

    Resize(w, h);
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


void Slider::OnKeyPressed(sf::Keyboard::Key key)
{
    switch (key)
    {
        case sf::Keyboard::Left:
            UpdateHandle(handle_index_ - quantum_);
            break;
        case sf::Keyboard::Right:
            UpdateHandle(handle_index_ + quantum_);
            break;
        case sf::Keyboard::Home:
            UpdateHandle(0);
            break;
        case sf::Keyboard::End:
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
            bar_.setFillColor(style.slider_bg_color);
            handle_.setFillColor(style.slider_handle_color);
            break;
        case State::FOCUSED:
            bar_.setFillColor(style.slider_bg_color_focus);
            handle_.setFillColor(style.slider_handle_color_focus);
            break;
        default:
            break;
    }
}


void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(bar_, states);
    target.draw(handle_, states);
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
    sf::Vector2f pos = handle_.getPosition();
    pos.x = (GetWidth() - GetHeight()) * handle_index_ / 100;
    handle_.setPosition(pos);
}

