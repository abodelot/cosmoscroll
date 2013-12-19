#ifndef ARCADEMENU_HPP
#define ARCADEMENU_HPP

#include "scenes/BaseMenu.hpp"

class ArcadeMenu: public BaseMenu
{
public:
	ArcadeMenu();

private:
	void EventCallback(int id) override;
};

#endif // ARCADEMENU_HPP
