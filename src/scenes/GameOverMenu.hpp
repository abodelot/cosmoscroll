#ifndef GAMEOVERMENU_HPP
#define GAMEOVERMENU_HPP

#include "BaseMenu.hpp"

class GameOverMenu: public BaseMenu
{
public:
	GameOverMenu();

	// inherited
	void Poke();

private:
	// inherited
	void EventCallback(int id);

	gui::Label* result_;
	CosmoButton* but_send_score_;
	gui::TextBox* txt_;
};

#endif // GAMEOVERMENU_HPP

