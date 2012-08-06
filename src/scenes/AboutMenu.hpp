#ifndef ABOUTMENU_HPP
#define ABOUTMENU_HPP

#include "BaseMenu.hpp"

class AboutMenu: public BaseMenu
{
public:
	AboutMenu();


private:
	// inherited
	void EventCallback(int id);
};

#endif // ABOUTMENU_HPP

