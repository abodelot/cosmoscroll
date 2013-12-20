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
	ConfigButton(gui::Menu* owner, Input::Action action = Input::COUNT);

	// Set button label based on keyboard key binded to the internal action
	void setKeyboardLabel();

	// Set button label based on joystick button binded to the internal action
	void setJoystickLabel();

	Input::Action getAction() const;

protected:
	void OnStateChanged(gui::State::EState state) override;

	void OnCallbackTriggered() override;

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Sprite    m_background;
	Input::Action m_action;
};


#endif // CONFIGBUTTON_HPP

