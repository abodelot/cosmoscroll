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

	// inherited
	void Show(sf::RenderTarget& target) const;

private:
	// inherited
	void Callback(int id);

	sf::Sprite title_;
};

#endif // MAINMENU_HPP

