#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP

#include "BaseMenu.hpp"

class SettingsMenu: public BaseMenu
{
public:
	SettingsMenu();

private:
	void EventCallback(int id);

	gui::CheckBox* cb_fullscreen_;
};

#endif // SETTINGSMENU_HPP

