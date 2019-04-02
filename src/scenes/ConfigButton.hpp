#ifndef CONFIGBUTTON_HPP
#define CONFIGBUTTON_HPP

#include "gui/Button.hpp"
#include "core/Input.hpp"

/**
 * Small sprite-based button for key-bindings menus
 */
class ConfigButton: public gui::Button
{
public:
    ConfigButton(gui::Menu* owner, Action::ID action = Action::NONE);

    // Set button label based on keyboard key binded to the internal action
    void setKeyboardLabel();

    // Set button label based on joystick button binded to the internal action
    void setJoystickLabel();

    Action::ID getAction() const;

protected:
    void OnStateChanged(gui::State::EState state) override;

    void OnCallbackTriggered() override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::Sprite m_background;
    Action::ID m_action;
    bool       m_error;
};


#endif // CONFIGBUTTON_HPP

