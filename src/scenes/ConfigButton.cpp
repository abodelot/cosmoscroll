#include "ConfigButton.hpp"
#include "core/Resources.hpp"

static constexpr int   BUTTON_WIDTH  = 120;
static constexpr int   BUTTON_HEIGHT = 25;
static const sf::Color ERROR_COLOR   = sf::Color(255, 200, 0);


ConfigButton::ConfigButton(gui::Menu* owner, Action::ID action):
    gui::Button(owner, "", BUTTON_WIDTH, BUTTON_HEIGHT),
    m_action(action),
    m_error(false)
{
    m_background.setTexture(Resources::getTexture("gui/button-config.png"));
    m_background.setTextureRect(sf::IntRect(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT));
    setTextSize(14);
    onStateChanged(getState());
}


void ConfigButton::setKeyboardLabel()
{
    sf::Keyboard::Key key = Input::getKeyBinding(m_action);
    setString(Input::keyToString(key));

    // Check for error and refresh label
    m_error = key == sf::Keyboard::Unknown;
    onStateChanged(getState());
}


void ConfigButton::setJoystickLabel()
{
    unsigned int button = Input::getButtonBinding(m_action);
    setString(Input::buttonToString(button));

    // Check for error and refresh label
    m_error = button == sf::Joystick::ButtonCount;
    onStateChanged(getState());
}


Action::ID ConfigButton::getAction() const
{
    return m_action;
}


void ConfigButton::onStateChanged(gui::State::EState state)
{
    gui::Button::onStateChanged(state);
    switch (state) {
        case gui::State::DEFAULT:
            m_background.setTextureRect(sf::IntRect(0, 0, BUTTON_WIDTH, BUTTON_HEIGHT));
            break;
        case gui::State::HOVERED:
            m_background.setTextureRect(sf::IntRect(0, BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT));
            break;
        case gui::State::FOCUSED:
            m_background.setTextureRect(sf::IntRect(0, BUTTON_HEIGHT * 2, BUTTON_WIDTH, BUTTON_HEIGHT));
            break;
        default:
            break;
    }

    if (m_error) {
        // Override label color when binding is invalid
        setTextColor(ERROR_COLOR);
    }
}


void ConfigButton::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_background, states);
    target.draw((gui::Button) *this);
}
