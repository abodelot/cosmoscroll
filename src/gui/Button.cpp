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

    const WidgetStyle& style = owner->GetWidgetStyle();
    text_.setCharacterSize(style.global_text_size);
    text_.setFont(*style.global_font);
    text_.setFillColor(style.button_text_color);
    text_.setOutlineColor(sf::Color::Black);
    text_.setOutlineThickness(1);
    OnStateChanged(GetState());
}


void Button::setString(const sf::String& text)
{
    text_.setString(text);
    centerText(text_);
}


void Button::setTextSize(int size)
{
    text_.setCharacterSize(size);
    centerText(text_);
}


void Button::setTextColor(const sf::Color& color)
{
    text_.setFillColor(color);
}


void Button::OnKeyPressed(sf::Keyboard::Key code)
{
    if (code == sf::Keyboard::Return)
    {
        triggerCallback();
    }
}


void Button::OnMouseClicked(int, int)
{
    triggerCallback();
}


void Button::OnStateChanged(State::EState state)
{
    switch (state)
    {
        case State::DEFAULT:
            text_.setFillColor(GetOwner()->GetWidgetStyle().button_text_color);
            break;
        case State::FOCUSED:
            text_.setFillColor(GetOwner()->GetWidgetStyle().button_text_color_focus);
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
