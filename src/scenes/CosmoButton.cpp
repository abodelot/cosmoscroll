#include "CosmoButton.hpp"
#include "core/Services.hpp"
#include "core/Resources.hpp"

static constexpr int BUTTON_WIDTH  = 220;
static constexpr int BUTTON_HEIGHT = 40;


CosmoButton::CosmoButton(gui::Menu* owner, const sf::String& text) :
    gui::Button(owner, text, BUTTON_WIDTH, BUTTON_HEIGHT),
    m_background(Resources::getTexture("gui/button.png"))
{
    m_background.setTextureRect(sf::IntRect(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT));
    setString(text);
    onStateChanged(getState());
}


void CosmoButton::onStateChanged(gui::State::EState state)
{
    switch (state) {
        case gui::State::DEFAULT:
            m_background.setTextureRect(sf::IntRect(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT));
            break;
        case gui::State::HOVERED:
            m_background.setTextureRect(sf::IntRect(0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT));
            Services::getSoundSystem().playSound("menu-select.ogg");
            break;
        case gui::State::FOCUSED:
            m_background.setTextureRect(sf::IntRect(0, BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT));
            break;
        default:
            break;
    }
    gui::Button::onStateChanged(state);
}


void CosmoButton::onCallbackTriggered()
{
    Services::getSoundSystem().playSound("menu-valid.ogg");
}


void CosmoButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_background, states);
    target.draw((gui::Button) *this);
}
