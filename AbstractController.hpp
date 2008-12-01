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
		USE_COOLER, EXIT_APP,
		COUNT_ACTION, NONE
	};

	enum Control
	{
		KEYBOARD = 1 << 0, JOY_0 = 1 << 1, JOY_1 = 1 << 2, ALL = (1 << 0) | (1 << 1) | (1 << 2)
	};
	
	bool GetAction(Action& action);
	bool HasInput(Action action);
	void SetControls(int c);

private:
	sf::Key::Code keyboard_binds_[COUNT_ACTION];
	typedef std::map<Action, unsigned int> JoyBindMap;
	JoyBindMap joystick_binds_;

	int cur_ctrls_;	// Le mode de controle courant.

	AbstractController();
	AbstractController(const AbstractController& other);
	//Settings& settings_;
};

typedef AbstractController AC;

#endif /* guard ABSTRACTCONTROLLER_HPP */

