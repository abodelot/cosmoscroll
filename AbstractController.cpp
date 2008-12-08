#include <cassert>

#include "AbstractController.hpp"
#include "Game.hpp"

#define JOY_ID			 0
#define JOY_DEADZONE	50.f


AbstractController& AbstractController::GetInstance()
{
    static AbstractController self;
    return self;
}


/*void AbstractController::SetControls(int controls)
{
	cur_ctrls_ = controls;
}*/


bool AbstractController::GetAction(Action& action, Device* device)
{
	static sf::RenderWindow& app_ = Game::GetInstance().GetApp();
	sf::Event event;
	Device _device;
	if (app_.GetEvent(event))
	{
		action = NONE;
		_device = ALL;
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
					_device = KEYBOARD;
					break;
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
					_device = JOYSTICK;
					break;
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
					_device = JOYSTICK;
				}
				else if (event.JoyMove.Position < -JOY_DEADZONE)
				{
					action = MOVE_LEFT;
					_device = JOYSTICK;
				}
			}
			else if (event.JoyMove.Axis == sf::Joy::AxisY)
			{
				if (event.JoyMove.Position > JOY_DEADZONE)
				{
					action = MOVE_DOWN;
					_device = JOYSTICK;
				}
				else if (event.JoyMove.Position < -JOY_DEADZONE)
				{
					action = MOVE_UP;
					_device = JOYSTICK;
				}				
			}
		}
		if (device != NULL)
		{
			*device = _device;
		}
		return true;
	}
	return false;
}


bool AbstractController::HasInput(Action action, int device)
{
	static const sf::Input& input_ = Game::GetInstance().GetInput();
	assert(action < COUNT_ACTION);
	if ((device & KEYBOARD) && input_.IsKeyDown(keyboard_binds_[action]))
	{
		return true;
	}
	if (device & JOYSTICK) 
	{
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
	}
	return false;
}

/*
void AbstractController::SetConfig(ConfigParser& config)
{
	std::string value;
	config.SeekSection("Keyboard");
	if (config.ReadItem("pause", value))
		keyboard_binds_[PAUSE] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("valid", value))
		keyboard_binds_[VALID] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("move_up", value))
		keyboard_binds_[MOVE_UP] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("move_down", value))
		keyboard_binds_[MOVE_DOWN] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("move_left", value))
		keyboard_binds_[MOVE_LEFT] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("move_right", value))
		keyboard_binds_[MOVE_RIGHT] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("weapon_1", value))
		keyboard_binds_[WEAPON_1] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("weapon_2", value))
		keyboard_binds_[WEAPON_2] = (sf::Key::Code) str_to_int(value);
	if (config.ReadItem("use_cooler", value))
		keyboard_binds_[USE_COOLER] = (sf::Key::Code) str_to_int(value);
}


void AbstractController::SaveConfig(ConfigParser& config) const
{
	config.SeekSection("Keyboard");
	config.UpdateItem("pause", keyboard_binds_[PAUSE]);
	config.UpdateItem("valid", keyboard_binds_[VALID]);
	config.UpdateItem("move_up", keyboard_binds_[MOVE_UP]);
	config.UpdateItem("move_down", keyboard_binds_[MOVE_DOWN]);
	config.UpdateItem("move_left", keyboard_binds_[MOVE_LEFT]);
	config.UpdateItem("move_right", keyboard_binds_[MOVE_RIGHT]);
	config.UpdateItem("weapon_1", keyboard_binds_[WEAPON_1]);
	config.UpdateItem("weapon_2", keyboard_binds_[WEAPON_2]);
	config.UpdateItem("use_cooler", keyboard_binds_[USE_COOLER]);
	
	config.SeekSection("Joystick");
	config.UpdateItem("pause", joystick_binds_[PAUSE]);
	config.UpdateItem("valid", joystick_binds_[VALID]);
	config.UpdateItem("weapon_1", joystick_binds_[WEAPON_1]);
	config.UpdateItem("weapon_2", joystick_binds_[WEAPON_2]);
	config.UpdateItem("use_cooler", joystick_binds_[USE_COOLER]);
}
*/

AbstractController::AbstractController()
{
	// TODO: différencier les touches réassignables de celles qui ne le sont pas
	// clavier
	keyboard_binds_[PAUSE] = sf::Key::P;
	keyboard_binds_[VALID] = sf::Key::Return;
	keyboard_binds_[MOVE_UP] = sf::Key::Up;
	keyboard_binds_[MOVE_DOWN] = sf::Key::Down;
	keyboard_binds_[MOVE_LEFT] = sf::Key::Left;
	keyboard_binds_[MOVE_RIGHT] = sf::Key::Right;
	keyboard_binds_[WEAPON_1] = sf::Key::Space;
	keyboard_binds_[WEAPON_2] = sf::Key::A;
	keyboard_binds_[USE_COOLER] = sf::Key::LControl;
	keyboard_binds_[EXIT_APP] = sf::Key::Escape;
	// joystick
	joystick_binds_[PAUSE] = 1;
	joystick_binds_[VALID] = 0;
	joystick_binds_[WEAPON_1] = 6;
	joystick_binds_[WEAPON_2] = 7;
	joystick_binds_[USE_COOLER] = 2;
	
	puts("AC initialisé");
}
