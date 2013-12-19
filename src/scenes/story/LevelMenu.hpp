#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "scenes/BaseMenu.hpp"
#include "CreditCounterWidget.hpp"

class LevelManager;

class LevelMenu: public BaseMenu
{
public:
	LevelMenu();

	void OnFocus() override;

private:
	void EventCallback(int id) override;

	sf::Text title_;
	LevelManager& levels_;
	CreditCounterWidget* m_credits;
	gui::OptionList* opt_levels_;
	gui::Label* lab_progresion_;
};

#endif // LEVELMENU_HPP

