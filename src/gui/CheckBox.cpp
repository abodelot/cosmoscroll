#include "CheckBox.hpp"
#include "Menu.hpp"

#define SIZE 20
#define PADDING 3
#define V_WIDTH 3

using namespace gui;

// TODO: gérer la taille du carré de la checkbox dans le style

CheckBox::CheckBox(Menu* owner) :
    Widget(owner, true)
{
    checked_ = false;
    const WidgetStyle& style = owner->GetWidgetStyle();

    box_.setSize(sf::Vector2f(SIZE, SIZE));
    box_.setFillColor(style.ckbox_bg_color);
    box_.setOutlineColor(style.global_border_color);
    box_.setOutlineThickness(1);

    float padding = PADDING;
    v1_.setSize(sf::Vector2f(SIZE - padding, padding));
    v1_.setOrigin(v1_.getSize().x / 2, v1_.getSize().y / 2);
    v1_.setPosition(SIZE / 2, SIZE / 2);
    v1_.setFillColor(style.ckbox_v_color);
    v1_.rotate(45);

    v2_.setSize(sf::Vector2f(padding, SIZE - padding));
    v2_.setOrigin(v2_.getSize().x / 2, v2_.getSize().y / 2);
    v2_.setPosition(SIZE / 2, SIZE / 2);
    v2_.setFillColor(style.ckbox_v_color);
    v2_.rotate(45);



    Resize(SIZE, SIZE);
    OnStateChanged(GetState());
}


bool CheckBox::Checked() const
{
    return checked_;
}


void CheckBox::Check(bool checked)
{
    checked_ = checked;
}


void CheckBox::OnKeyPressed(sf::Keyboard::Key code)
{
    if (code == sf::Keyboard::Return)
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
            box_.setFillColor(style.ckbox_bg_color);
            v1_.setFillColor(style.ckbox_v_color);
            v2_.setFillColor(style.ckbox_v_color);
            break;
        case State::FOCUSED:
            box_.setFillColor(style.ckbox_bg_color_focus);
            v1_.setFillColor(style.ckbox_v_color_focus);
            v2_.setFillColor(style.ckbox_v_color_focus);
            break;
        default:
            break;
    }
}


void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(box_, states);
    if (checked_) {
        target.draw(v1_, states);
        target.draw(v2_, states);
    }
}
