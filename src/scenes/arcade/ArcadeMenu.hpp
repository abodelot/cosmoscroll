#ifndef ARCADEMENU_HPP
#define ARCADEMENU_HPP

#include "scenes/BaseMenu.hpp"

class ArcadeMenu: public BaseMenu
{
public:
	ArcadeMenu();
private:
	// override
	void EventCallback(int id);
};

#endif // ARCADEMENU_HPP
