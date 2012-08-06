#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "CreditCounterBase.hpp"

class LevelManager;

class LevelMenu: public CreditCounterBase
{
public:
	LevelMenu();

	// override
	void OnFocus();

private:
	// override
	void EventCallback(int id);

	sf::Text title_;
	LevelManager& levels_;
	gui::OptionList* opt_levels_;
	gui::Label* lab_progresion_;
	gui::Label* lab_hardcore_;
	gui::CheckBox* cbx_hardcore_;
};

#endif // LEVELMENU_HPP

