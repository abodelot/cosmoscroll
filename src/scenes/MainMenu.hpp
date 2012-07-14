#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "BaseMenu.hpp"

/**
 * Menu principal de l'application
 */
class MainMenu: public BaseMenu
{
public:
	MainMenu();

	// override
	void Show(sf::RenderTarget& target) const;

private:
	// override
	void EventCallback(int id);

	sf::Sprite title_;
};

#endif // MAINMENU_HPP

