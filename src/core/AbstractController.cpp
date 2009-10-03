#include <cassert>

#include "AbstractController.hpp"
#include "../core/Game.hpp"

#define JOY_ID			 0
#define JOY_DEADZONE	50.f


AbstractController& AbstractController::GetInstance()
{
    static AbstractController self;
    return self;
}


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


void AbstractController::LoadConfig(ConfigParser& config)
{
	config.SeekSection("Keyboard");
	config.ReadItem("pause", keyboard_binds_[PAUSE]);
	//config.ReadItem("valid", keyboard_binds_[VALID]);
	config.ReadItem("move_up", keyboard_binds_[MOVE_UP]);
	config.ReadItem("move_down", keyboard_binds_[MOVE_DOWN]);
	config.ReadItem("move_left", keyboard_binds_[MOVE_LEFT]);
	config.ReadItem("move_right", keyboard_binds_[MOVE_RIGHT]);
	config.ReadItem("weapon_1", keyboard_binds_[WEAPON_1]);
	config.ReadItem("weapon_2", keyboard_binds_[WEAPON_2]);
	config.ReadItem("use_cooler", keyboard_binds_[USE_COOLER]);

	config.SeekSection("Joystick");
	config.ReadItem("pause", joystick_binds_[PAUSE]);
	config.ReadItem("valid", joystick_binds_[VALID]);
	config.ReadItem("weapon_1", joystick_binds_[WEAPON_1]);
	config.ReadItem("weapon_2", joystick_binds_[WEAPON_2]);
	config.ReadItem("use_cooler", joystick_binds_[USE_COOLER]);
}


void AbstractController::SaveConfig(ConfigParser& config) const
{
	config.SeekSection("Keyboard");
	config.WriteItem("pause", keyboard_binds_[PAUSE]);
	//config.WriteItem("valid", keyboard_binds_[VALID]);
	config.WriteItem("move_up", keyboard_binds_[MOVE_UP]);
	config.WriteItem("move_down", keyboard_binds_[MOVE_DOWN]);
	config.WriteItem("move_left", keyboard_binds_[MOVE_LEFT]);
	config.WriteItem("move_right", keyboard_binds_[MOVE_RIGHT]);
	config.WriteItem("weapon_1", keyboard_binds_[WEAPON_1]);
	config.WriteItem("weapon_2", keyboard_binds_[WEAPON_2]);
	config.WriteItem("use_cooler", keyboard_binds_[USE_COOLER]);

	config.SeekSection("Joystick");
	config.WriteItem("pause", joystick_binds_[PAUSE]);
	config.WriteItem("valid", joystick_binds_[VALID]);
	config.WriteItem("weapon_1", joystick_binds_[WEAPON_1]);
	config.WriteItem("weapon_2", joystick_binds_[WEAPON_2]);
	config.WriteItem("use_cooler", joystick_binds_[USE_COOLER]);
}


unsigned int AbstractController::GetBinding(Action action, Device device) const
{
	switch (device)
	{
		case KEYBOARD:
			return keyboard_binds_[action];
		case JOYSTICK:
			return joystick_binds_[action];
		default:
			assert(0);
	}
}

void AbstractController::SetBinding(Action action, Device device, unsigned int binding)
{
	switch (device)
	{
		case KEYBOARD:
			keyboard_binds_[action] = (sf::Key::Code) binding;
			break;
		case JOYSTICK:
			joystick_binds_[action] = binding;
			break;
		default:
			assert(0);
	}
}


const char* AbstractController::KeyToString(int code)
{
	using namespace sf::Key;
	switch (code)
	{
		case A: return "A";
		case B: return "B";
		case C: return "C";
		case D: return "D";
		case E: return "E";
		case F: return "F";
		case G: return "G";
		case H: return "H";
		case I: return "I";
		case J: return "J";
		case K: return "K";
		case L: return "L";
		case M: return "M";
		case N: return "N";
		case O: return "O";
		case P: return "P";
		case Q: return "Q";
		case R: return "R";
		case S: return "S";
		case T: return "T";
		case U: return "U";
		case V: return "V";
		case W: return "W";
		case X: return "X";
		case Y: return "Y";
		case Z: return "Z";
		case Num0: return "Num0";
		case Num1: return "Num1";
		case Num2: return "Num2";
		case Num3: return "Num3";
		case Num4: return "Num4";
		case Num5: return "Num5";
		case Num6: return "Num6";
		case Num7: return "Num7";
		case Num8: return "Num8";
		case Num9: return "Num9";
		case sf::Key::Escape: return "Escape";	// Un Escape est déja défini dans minGW-> Ambiguité.
		case LControl: return "LControl";
		case LShift: return "LShift";
		case LAlt: return "LAlt";
		case LSystem: return "LSystem";
		case RControl: return "RControl";
		case RShift: return "RShift";
		case RAlt: return "RAlt";
		case RSystem: return "RSystem";
		case Menu: return "Menu";
		case LBracket: return "LBracket";
		case RBracket: return "RBracket";
		case SemiColon: return "SemiColon";
		case Comma: return "Comma";
		case Period: return "Period";
		case Quote: return "Quote";
		case Slash: return "Slash";
		case BackSlash: return "BackSlash";
		case Tilde: return "Tilde";
		case Equal: return "Equal";
		case Dash: return "Dash";
		case Space: return "Space";
		case Return: return "Return";
		case Back: return "Back";
		case Tab: return "Tab";
		case PageUp: return "PageUp";
		case PageDown: return "PageDown";
		case End: return "End";
		case Home: return "Home";
		case Insert: return "Insert";
		case Delete: return "Delete";
		case Add: return "Add";
		case Subtract: return "Subtract";
		case Multiply: return "Multiply";
		case Divide: return "Divide";
		case Left: return "Left";
		case Right: return "Right";
		case Up: return "Up";
		case Down: return "Down";
		case Numpad0: return "Numpad0";
		case Numpad1: return "Numpad1";
		case Numpad2: return "Numpad2";
		case Numpad3: return "Numpad3";
		case Numpad4: return "Numpad4";
		case Numpad5: return "Numpad5";
		case Numpad6: return "Numpad6";
		case Numpad7: return "Numpad7";
		case Numpad8: return "Numpad8";
		case Numpad9: return "Numpad9";
		case F1: return "F1";
		case F2: return "F2";
		case F3: return "F3";
		case F4: return "F4";
		case F5: return "F5";
		case F6: return "F6";
		case F7: return "F7";
		case F8: return "F8";
		case F9: return "F9";
		case F10: return "F10";
		case F11: return "F11";
		case F12: return "F12";
		case F13: return "F13";
		case F14: return "F14";
		case F15: return "F15";
		case Pause: return "Pause";

		default: return "<Unknown key>";
	}
}


AbstractController::AbstractController()
{
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
	keyboard_binds_[TAKE_SCREENSHOT] = sf::Key::F1;
	keyboard_binds_[PANEL_UP] = sf::Key::PageUp;
	keyboard_binds_[PANEL_DOWN] = sf::Key::PageDown;
	keyboard_binds_[EXIT_APP] = sf::Key::Escape;
	// joystick
	joystick_binds_[PAUSE] = 1;
	joystick_binds_[VALID] = 0;
	joystick_binds_[WEAPON_1] = 6;
	joystick_binds_[WEAPON_2] = 7;
	joystick_binds_[USE_COOLER] = 2;
}


std::istream& operator>>(std::istream& in, sf::Key::Code& code)
{
	int i;
	in >> i;
	code = static_cast<sf::Key::Code>(i);
	return in;
}

