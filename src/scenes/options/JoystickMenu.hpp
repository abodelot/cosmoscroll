#ifndef JOYSTICKMENU_HPP
#define JOYSTICKMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "core/Input.hpp"

/**
 * Menu de configuration du joystick
 */
class JoystickMenu: public BaseMenu
{
public:
	JoystickMenu();

	// inherited
	void OnFocus();

private:
	std::wstring GetButtonLabel(Input::Action action) const;

	// inherited
	void EventCallback(int id);

	gui::Button* but_weapon1_;
	gui::Button* but_weapon2_;
	gui::Button* but_missile_;
	gui::Button* but_cooler_;
	gui::Button* but_pause_;
	gui::Slider* sl_joystick_;
};

#endif // JOYSTICKMENU_HPP

