#ifndef PAUSEMENU_HPP
#define PAUSEMENU_HPP

#include "BaseMenu.hpp"

class PauseMenu: public BaseMenu
{
public:
	PauseMenu();

	// inherited
	void Update(float frametime);

	// inherited
	void Show(sf::RenderTarget& target) const;

private:
	// inherited
	void Callback(int id);
};

#endif // PAUSEMENU_HPP

