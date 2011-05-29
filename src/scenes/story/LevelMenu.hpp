#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "scenes/BaseMenu.hpp"
#include <vector>

class LevelManager;

class LevelMenu: public BaseMenu
{
public:
	LevelMenu();

	// override
	void OnFocus();

	// override
	void Show(sf::RenderTarget& target) const;

private:
	// override
	void EventCallback(int id);

	sf::String title_;
	LevelManager& levels_;
	gui::OptionList* opt_levels_;
	gui::Label* lab_progresion_;
	gui::Label* lab_hardcore_;
	gui::CheckBox* cbx_hardcore_;

	sf::Sprite credit_counter_bg_;
	sf::String credit_counter_;
};

#endif // LEVELMENU_HPP

