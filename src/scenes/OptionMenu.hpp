#ifndef OPTIONMENU_HPP
#define OPTIONMENU_HPP

#include "BaseMenu.hpp"

class OptionMenu: public BaseMenu
{
public:
	OptionMenu();

private:
	// inherited
	void Callback(int id);
};

#endif // OPTIONMENU_HPP

