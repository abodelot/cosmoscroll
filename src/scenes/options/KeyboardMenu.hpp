#ifndef KEYBOARDMENU_HPP
#define KEYBOARDMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "core/Input.hpp"

/**
 * Menu de configuration du clavier
 */
class KeyboardMenu: public BaseMenu
{
public:
	KeyboardMenu();

	// inherited
	void OnFocus();

private:

	// inherited
	void EventCallback(int id);

	void AddRow(gui::FormLayout& form, Input::Action action, gui::Button** button);
	const char* GetKeyLabel(Input::Action action) const;

	gui::Button* but_up_;
	gui::Button* but_down_;
	gui::Button* but_left_;
	gui::Button* but_right_;
	gui::Button* but_weapon1_;
	gui::Button* but_weapon2_;
	gui::Button* but_cooler_;
	gui::Button* but_missile_;
	gui::Button* but_pause_;
};

#endif // KEYBOARDMENU_HPP

