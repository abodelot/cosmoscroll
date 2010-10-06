#ifndef ENDGAMEMENU_HPP
#define ENDGAMEMENU_HPP

#include "BaseMenu.hpp"

class EndGameMenu: public BaseMenu
{
public:
	EndGameMenu();

	void Show(sf::RenderTarget& target) const;

	// inherited
	void Poke();

private:
	// inherited
	void EventCallback(int id);

	sf::String result_;
};

#endif // ENDGAMEMENU_HPP

