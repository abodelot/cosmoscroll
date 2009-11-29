#ifndef LEVELMENU_HPP
#define LEVELMENU_HPP

#include "BaseMenu.hpp"

class LevelManager;

class LevelMenu: public BaseMenu
{
public:
	LevelMenu();

	// inherited
	void Poke();

private:
	// inherited
	void Callback(int id);

	sf::String title_;
	LevelManager& levels_;
};

#endif // LEVELMENU_HPP

