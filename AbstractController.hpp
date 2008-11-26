#ifndef ABSTRACTCONTROLLER_HPP
#define ABSTRACTCONTROLLER_HPP

#include <map>
#include <SFML/Graphics.hpp>
class AbstractController
{
public:
	static AbstractController& GetInstance();
	
	// tous les évènements gérés par le jeu
	enum Action
	{
		PAUSE, VALID,
		MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT, WEAPON_1, WEAPON_2,
		USE_HACK, USE_COOLER, EXIT_APP,
		COUNT_ACTION, NONE
	};
	
	bool GetAction(Action& action);
	bool HasInput(Action action);


/*	void SetKeyboardAction(Action action, sf::Key::Code code);

	void SetKeyboardMove(Movement movement, sf::Key::Code code);

	void SetJoystickAction(Action action, sf::JoystickButton::Button button);

	void SetJoystickMove(Movement movement, sf::Joy::Axis axis);*/

private:
	sf::Key::Code keyboard_binds_[COUNT_ACTION];
	typedef std::map<Action, unsigned int> JoyBindMap;
	JoyBindMap joystick_binds_;

	AbstractController();
	AbstractController(const AbstractController& other);
	//Settings& settings_;
};

typedef AbstractController AC;

#endif /* guard ABSTRACTCONTROLLER_HPP */

