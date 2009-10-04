#ifndef ABSTRACTCONTROLLER_HPP
#define ABSTRACTCONTROLLER_HPP

#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "../utils/ConfigParser.hpp"

/**
 * Singleton du gestionnaire des périphériques d'entrée
 */
class AbstractController
{
public:
	static AbstractController& GetInstance();

	// tous les types d'évènements gérés par le jeu
	enum Action
	{
		EXIT_APP,
		PAUSE,
		VALID,
		MOVE_UP,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		WEAPON_1,
		WEAPON_2,
		USE_COOLER,
		USE_MISSILE,
		TAKE_SCREENSHOT,
		PANEL_UP,
		PANEL_DOWN,
		COUNT_ACTION,
		NONE
	};

	enum Device
	{
		KEYBOARD = 1 << 1,
		JOYSTICK = 1 << 2,
		ALL      = KEYBOARD | JOYSTICK
	};

	/**
	 * Dépiler les évènements
	 * @param[out] action: évènement à récupérer
	 * @return true si la pile d'évènements est vide, sinon false
	 */
	bool GetAction(Action& action, Device* device = NULL);

	/**
	 * @param[in] action: évènement à tester
	 * @param[in] device: flag
	 */
	bool HasInput(Action action, int device = ALL);

	/**
	 * Charger les bindings depuis un fichier de configuration
	 * @param[in] config: configuration contenant les bindings
	 */
	void LoadConfig(ConfigParser& config);

	/**
	 * Sauvegarder les bindings dans un fichier de configuration
	 * @param[out] config: configuration où écrire les bindings
	 */
	void SaveConfig(ConfigParser& config) const;

	/**
	 * Récupérer le code SFML d'un binding
	 */
	unsigned int GetBinding(Action action, Device device) const;

	/**
	 * Spécifier un code SFML pour un binding
	 */
	void SetBinding(Action action, Device device, unsigned int binding);

	/**
	 * Obtenir la repésentation textuelle d'une touche
	 * @param[in] key: key code sfml de la touche
	 */
	static const char* KeyToString(int key);

private:
	AbstractController();
	AbstractController(const AbstractController& other);

	sf::Key::Code keyboard_binds_[COUNT_ACTION];

	typedef std::map<Action, unsigned int> JoyBindMap;
	mutable JoyBindMap joystick_binds_;

};

typedef AbstractController AC; // lazy typedef


std::istream& operator>>(std::istream& in, sf::Key::Code& code);


#endif /* guard ABSTRACTCONTROLLER_HPP */

