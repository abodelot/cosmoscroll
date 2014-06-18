#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "BaseMenu.hpp"

/**
 * Application main menu
 */
class MainMenu: public BaseMenu
{
public:
	MainMenu();

	void Show(sf::RenderTarget& target) const override;

private:
	void EventCallback(int id) override;

	sf::Sprite title_;
};

#endif // MAINMENU_HPP

