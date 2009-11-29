#ifndef KEYBOARDMENU_HPP
#define KEYBOARDMENU_HPP

#include "BaseMenu.hpp"
#include "../core/Input.hpp"

/**
 * Menu de configuration du clavier
 */
class KeyboardMenu: public BaseMenu
{
public:
	KeyboardMenu();

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

#endif // KEYBOARDMENU_HPP

