#ifndef ABOUTMENU_HPP
#define ABOUTMENU_HPP

#include "BaseMenu.hpp"

class AboutMenu: public BaseMenu
{
public:
	AboutMenu();

private:
	void EventCallback(int id) override;
};

#endif // ABOUTMENU_HPP

