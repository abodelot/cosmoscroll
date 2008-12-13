#ifndef ABSTRACTCONTROLLER_HPP
#define ABSTRACTCONTROLLER_HPP

#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "ConfigParser.hpp"


class AbstractController
{
public:
	static AbstractController& GetInstance();
	
	// tous les types d'évènements gérés par le jeu
	enum Action
	{
		EXIT_APP, PAUSE, VALID,
		MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, 
		WEAPON_1, WEAPON_2, USE_COOLER,
		COUNT_ACTION, NONE
	};
	
	enum Device
	{
		KEYBOARD = 1 << 1,
		JOYSTICK = 1 << 2,
		ALL      = KEYBOARD | JOYSTICK
	};
	
	/**
	 * @param[out] action: évènement à récupérer
	 */
	bool GetAction(Action& action, Device* device = NULL);
	
	/**
	 * @param[in] action: évènement à tester
	 * @param[in] device: flag
	 */
	bool HasInput(Action action, int device = ALL);
	
	/**
	 * Charger les bindings depuis un fichier de configuration
	 */
	void LoadConfig(ConfigParser& config);
	
	/**
	 * Sauvegarder les bindings dans un fichier de configuration
	 */
	void SaveConfig(ConfigParser& config) const;
	
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

