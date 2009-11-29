#ifndef JOYSTICKMENU_HPP
#define JOYSTICKMENU_HPP

#include "BaseMenu.hpp"

/**
 * Menu de configuration du joystick
 */
class JoystickMenu: public BaseMenu
{
public:
	JoystickMenu();

	// inherited
	void Poke();

private:
	/**
	 * Ajouter une action à binder en option dans le menu
	 */
	void AddBindOpt(Input::Action action);

	// inherited
	void Callback(int id);
};

#endif // JOYSTICKMENU_HPP

