#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "CreditCounterBase.hpp"

class LevelManager;

class LevelMenu: public CreditCounterBase
{
public:
	LevelMenu();

	void OnFocus() override;

private:
	void EventCallback(int id) override;

	sf::Text title_;
	LevelManager& levels_;
	gui::OptionList* opt_levels_;
	gui::Label* lab_progresion_;
};

#endif // LEVELMENU_HPP

