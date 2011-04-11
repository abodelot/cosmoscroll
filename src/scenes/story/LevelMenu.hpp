#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "scenes/BaseMenu.hpp"
#include <vector>

class LevelManager;

class LevelMenu: public BaseMenu
{
public:
	LevelMenu();

	// inherited
	void OnFocus();

private:
	// inherited
	void EventCallback(int id);

	sf::String title_;
	LevelManager& levels_;
	gui::OptionList* opt_levels_;
	gui::Label* lab_progresion_;
};

#endif // LEVELMENU_HPP

