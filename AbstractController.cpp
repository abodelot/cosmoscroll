#include <cassert>

#include "AbstractController.hpp"
#include "Game.hpp"

#define JOY_ID			 0
#define JOY_DEADZONE	15.f

AbstractController& AbstractController::GetInstance()
{
    static AbstractController self;
    return self;
}


bool AbstractController::GetAction(Action& action)
{
	static sf::RenderWindow& app_ = Game::GetInstance().GetApp();
	sf::Event event;
	if (app_.GetEvent(event))
	{
		action = NONE;
		// CLOSE
		if (event.Type == sf::Event::Closed)
		{
			action = EXIT_APP;
		}
		// KEY PRESSED
		else if (event.Type == sf::Event::KeyPressed)
		{
			for (int i = 0; i < COUNT_ACTION; ++i)
			{
				if (event.Key.Code == keyboard_binds_[i])
				{
					action = (Action) i;
				}
			}
		}
		// JOYBUTTON PRESSED
		else if (event.Type == sf::Event::JoyButtonPressed)
		{
			for (JoyBindMap::const_iterator it = joystick_binds_.begin();
				it != joystick_binds_.end(); ++it)
			{
				if (it->second == event.JoyButton.Button)
				{
					action = it->first;
				}
			}
		}
		// JOYAXIS MOVED
		else if (event.Type == sf::Event::JoyMoved)
		{
			if (event.JoyMove.Axis == sf::Joy::AxisX)
			{
				if (event.JoyMove.Position > JOY_DEADZONE)
				{
					action = MOVE_RIGHT;
				}
				else if (event.JoyMove.Position < - JOY_DEADZONE)
				{
					action = MOVE_LEFT;
				}
			}
			else if (event.JoyMove.Axis == sf::Joy::AxisY)
			{
				if (event.JoyMove.Position > JOY_DEADZONE)
				{
					action = MOVE_DOWN;
				}
				else if (event.JoyMove.Position < - JOY_DEADZONE)
				{
					action = MOVE_UP;
				}				
			}
		}
		return true;
	}
	return false;
}


bool AbstractController::HasInput(Action action)
{
	static const sf::Input& input_ = Game::GetInstance().GetInput();
	assert(action < COUNT_ACTION);
	if (input_.IsKeyDown(keyboard_binds_[action]))
	{
		return true;
	}
	if (input_.IsJoystickButtonDown(JOY_ID, joystick_binds_[action]))
	{
		return true;
	}
	if (action == MOVE_UP)
	{
		return input_.GetJoystickAxis(JOY_ID, sf::Joy::AxisY) < -JOY_DEADZONE;
	}
	if (action == MOVE_DOWN)
	{
		return input_.GetJoystickAxis(JOY_ID, sf::Joy::AxisY) > JOY_DEADZONE;
	}
	if (action == MOVE_LEFT)
	{
		return input_.GetJoystickAxis(JOY_ID, sf::Joy::AxisX) < -JOY_DEADZONE;
	}
	if (action == MOVE_RIGHT)
	{
		return input_.GetJoystickAxis(JOY_ID, sf::Joy::AxisX) > JOY_DEADZONE;
	}
	return false;
}


AbstractController::AbstractController()
{
	// temporaire : on code en dur toutes les touches ! (les réassignables comme les fixes)
	// (on court-circuite Settings)
	// TODO: différencier les touches réassignables de celles qui ne le sont pas
	puts("start AC init");
		
	// clavier
	keyboard_binds_[PAUSE] = sf::Key::P;
	keyboard_binds_[VALID] = sf::Key::Return;
	keyboard_binds_[MOVE_UP] = sf::Key::Up;
	keyboard_binds_[MOVE_DOWN] = sf::Key::Down;
	keyboard_binds_[MOVE_LEFT] = sf::Key::Left;
	keyboard_binds_[MOVE_RIGHT] = sf::Key::Right;
	keyboard_binds_[WEAPON_1] = sf::Key::Space;
	keyboard_binds_[WEAPON_2] = sf::Key::A;
	keyboard_binds_[USE_HACK] = sf::Key::H;
	keyboard_binds_[USE_COOLER] = sf::Key::LControl;
	keyboard_binds_[EXIT_APP] = sf::Key::Escape;
	// joystick
	joystick_binds_[PAUSE] = 1;
	joystick_binds_[VALID] = 5;
	joystick_binds_[WEAPON_1] = 2;
	joystick_binds_[WEAPON_2] = 3;
	joystick_binds_[USE_COOLER] = 4;
	
	puts("end AC init");
}
