#ifndef ABOUTMENU_HPP
#define ABOUTMENU_HPP

#include "BaseMenu.hpp"

class AboutMenu: public BaseMenu
{
public:
	AboutMenu();

	// inherited
	void Show(sf::RenderTarget& target) const;

private:
	// inherited
	void Callback(int id);

	sf::String info_text_;
	sf::Sprite logos_;
};

#endif // ABOUTMENU_HPP

