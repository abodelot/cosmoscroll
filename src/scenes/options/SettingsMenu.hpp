#ifndef SETTINGSMENU_HPP
#define SETTINGSMENU_HPP

#include "scenes/BaseMenu.hpp"

class SettingsMenu: public BaseMenu
{
public:
	SettingsMenu();

private:
	void EventCallback(int id);

	gui::CheckBox* cb_fullscreen_;
	gui::OptionList* opt_languages_;
	gui::Label* lab_fullscreen_;
	gui::Label* lab_language_;
	gui::Button* but_back_;
};

#endif // SETTINGSMENU_HPP

