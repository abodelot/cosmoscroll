#ifndef JOYSTICKMENU_HPP
#define JOYSTICKMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "scenes/ConfigButton.hpp"
#include "core/Input.hpp"

/**
 * Menu de configuration du joystick
 */
class JoystickMenu: public BaseMenu
{
public:
	JoystickMenu();

	void OnEvent(const sf::Event& event) override;
	void OnFocus() override;

private:
	void EventCallback(int id) override;

	ConfigButton* addRow(gui::FormLayout& form, Action::ID action);

	ConfigButton* m_triggered;
	ConfigButton* but_weapon_;
	ConfigButton* but_missile_;
	ConfigButton* but_cooler_;
	ConfigButton* but_pause_;
	gui::Slider* sl_joystick_;
};

#endif // JOYSTICKMENU_HPP

