#ifndef ENDGAMEMENU_HPP
#define ENDGAMEMENU_HPP

#include "BaseMenu.hpp"

class EndGameMenu: public BaseMenu
{
public:
	EndGameMenu();

	// inherited
	void Poke();

private:
	// inherited
	void EventCallback(int id);

	gui::Label* result_;
	CosmoButton* but_send_score_;
	gui::TextBox* txt_;
};

#endif // ENDGAMEMENU_HPP

