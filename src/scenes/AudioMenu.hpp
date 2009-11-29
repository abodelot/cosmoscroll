#ifndef AUDIOMENU_HPP
#define AUDIOMENU_HPP

#include "BaseMenu.hpp"

class AudioMenu: public BaseMenu
{
public:
	AudioMenu();

private:
	void Callback(int id);
};

#endif // AUDIOMENU_HPP

