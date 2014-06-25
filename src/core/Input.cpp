#include <sstream>
#include <iostream>
#include "Input.hpp"
#include "utils/I18n.hpp"

Input::KeyMap    Input::s_keys;
Input::ButtonMap Input::s_buttons;
Input::ActionMap Input::s_pressed;
int              Input::s_joystick_deadzone = 50;
Input::Init      Input::s_ctor;


Input::Init::Init()
{
	// Default keyboard layout
	Input::setKeyBinding(sf::Keyboard::Return,   Action::VALIDATE);
	Input::setKeyBinding(sf::Keyboard::Up,       Action::UP);
	Input::setKeyBinding(sf::Keyboard::Down,     Action::DOWN);
	Input::setKeyBinding(sf::Keyboard::Left,     Action::LEFT);
	Input::setKeyBinding(sf::Keyboard::Right,    Action::RIGHT);
	Input::setKeyBinding(sf::Keyboard::PageUp,   Action::PANEL_UP);
	Input::setKeyBinding(sf::Keyboard::PageDown, Action::PANEL_DOWN);
	Input::setKeyBinding(sf::Keyboard::Escape,   Action::PAUSE);
	Input::setKeyBinding(sf::Keyboard::Space,    Action::USE_LASER);
	Input::setKeyBinding(sf::Keyboard::LControl, Action::USE_COOLER);
	Input::setKeyBinding(sf::Keyboard::A,        Action::USE_MISSILE);
	Input::setKeyBinding(sf::Keyboard::F1,       Action::TAKE_SCREENSHOT);

	// Default joystick layout
	Input::setButtonBinding(0, Action::VALIDATE);
	Input::setButtonBinding(1, Action::PAUSE);
	Input::setButtonBinding(6, Action::USE_LASER);
	Input::setButtonBinding(2, Action::USE_COOLER);
	Input::setButtonBinding(3, Action::USE_MISSILE);
}


Action::ID Input::matchKey(sf::Keyboard::Key key)
{
	KeyMap::const_iterator it = s_keys.find(key);
	return it != s_keys.end() ? it->second : Action::NONE;
}


Action::ID Input::matchButton(unsigned int button)
{
	ButtonMap::const_iterator it = s_buttons.find(button);
	return it != s_buttons.end() ? it->second : Action::NONE;
}


Action::ID Input::feedEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
		{
			Action::ID action = matchKey(event.key.code);
			if (action != Action::NONE)
				s_pressed[action] = true;

			return action;
		}
		case sf::Event::KeyReleased:
		{
			Action::ID action = matchKey(event.key.code);
			if (action != Action::NONE)
				s_pressed[action] = false;

			break;
		}
		case sf::Event::JoystickButtonPressed:
		{
			Action::ID action = matchButton(event.joystickButton.button);
			if (action != Action::NONE)
				s_pressed[action] = true;

			return action;
		}
		case sf::Event::JoystickButtonReleased:
		{
			Action::ID action = matchButton(event.joystickButton.button);
			if (action != Action::NONE)
				s_pressed[action] = false;

			break;
		}
		case sf::Event::JoystickMoved:
			if (event.joystickMove.axis == sf::Joystick::X)
			{
				if (event.joystickMove.position > s_joystick_deadzone)
				{
					s_pressed[Action::RIGHT] = true;
					return Action::RIGHT;
				}
				s_pressed[Action::RIGHT] = false;

				if (event.joystickMove.position < -s_joystick_deadzone)
				{
					s_pressed[Action::LEFT] = true;
					return Action::LEFT;
				}
				s_pressed[Action::LEFT] = false;
			}
			else if (event.joystickMove.axis == sf::Joystick::Y)
			{
				if (event.joystickMove.position > s_joystick_deadzone)
				{
					s_pressed[Action::DOWN] = true;
					return Action::DOWN;
				}
				s_pressed[Action::DOWN] = false;

				if (event.joystickMove.position < -s_joystick_deadzone)
				{
					s_pressed[Action::UP] = true;
					return Action::UP;
				}
				s_pressed[Action::UP] = false;
			}
			break;

		case sf::Event::Closed:
			return Action::EXIT_APP;

		default:
			break;
	}
	return Action::NONE;
}


bool Input::isPressed(Action::ID action)
{
	return s_pressed[action];
}


void Input::setKeyBinding(sf::Keyboard::Key key, Action::ID action)
{
	if (key < sf::Keyboard::KeyCount)
	{
		// Disable old key
		s_keys[getKeyBinding(action)] = Action::NONE;
		// Bind new key
		s_keys[key] = action;
		s_pressed[action] = false;
	}
	else
	{
		std::cerr << "[Input] invalid key binding ignored: " << key << std::endl;
	}
}


sf::Keyboard::Key Input::getKeyBinding(Action::ID action)
{
	for (auto& item: s_keys)
	{
		if (item.second == action)
			return item.first;
	}
	return sf::Keyboard::Unknown;
}


void Input::setButtonBinding(unsigned int joystick_button, Action::ID action)
{
	if (joystick_button < sf::Joystick::ButtonCount)
	{
		// Disable old button
		s_buttons[getButtonBinding(action)] = Action::NONE;
		// Bind new button
		s_buttons[joystick_button] = action;
		s_pressed[action] = false;
	}
	else
	{
		std::cerr << "[Input] invalid joystick button binding ignored: "  << joystick_button << std::endl;
	}
}


unsigned int Input::getButtonBinding(Action::ID action)
{
	for (auto& item: s_buttons)
	{
		if (item.second == action)
			return item.first;
	}
	return sf::Joystick::ButtonCount;
}


void Input::setJoystickDeadzone(int dead_zone)
{
	s_joystick_deadzone = dead_zone;
}


int Input::getJoystickDeadzone()
{
	return s_joystick_deadzone;
}


const char* Input::keyToString(sf::Keyboard::Key key)
{
	switch (key)
	{
		case sf::Keyboard::A:         return "A";
		case sf::Keyboard::B:         return "B";
		case sf::Keyboard::C:         return "C";
		case sf::Keyboard::D:         return "D";
		case sf::Keyboard::E:         return "E";
		case sf::Keyboard::F:         return "F";
		case sf::Keyboard::G:         return "G";
		case sf::Keyboard::H:         return "H";
		case sf::Keyboard::I:         return "I";
		case sf::Keyboard::J:         return "J";
		case sf::Keyboard::K:         return "K";
		case sf::Keyboard::L:         return "L";
		case sf::Keyboard::M:         return "M";
		case sf::Keyboard::N:         return "N";
		case sf::Keyboard::O:         return "O";
		case sf::Keyboard::P:         return "P";
		case sf::Keyboard::Q:         return "Q";
		case sf::Keyboard::R:         return "R";
		case sf::Keyboard::S:         return "S";
		case sf::Keyboard::T:         return "T";
		case sf::Keyboard::U:         return "U";
		case sf::Keyboard::V:         return "V";
		case sf::Keyboard::W:         return "W";
		case sf::Keyboard::X:         return "X";
		case sf::Keyboard::Y:         return "Y";
		case sf::Keyboard::Z:         return "Z";
		case sf::Keyboard::Num0:      return "0";
		case sf::Keyboard::Num1:      return "1";
		case sf::Keyboard::Num2:      return "2";
		case sf::Keyboard::Num3:      return "3";
		case sf::Keyboard::Num4:      return "4";
		case sf::Keyboard::Num5:      return "5";
		case sf::Keyboard::Num6:      return "6";
		case sf::Keyboard::Num7:      return "7";
		case sf::Keyboard::Num8:      return "8";
		case sf::Keyboard::Num9:      return "9";
		case sf::Keyboard::Escape:    return "Escape";
		case sf::Keyboard::LControl:  return "Left Control";
		case sf::Keyboard::LShift:    return "Left Shift";
		case sf::Keyboard::LAlt:      return "Left Alt";
		case sf::Keyboard::LSystem:   return "Left System";
		case sf::Keyboard::RControl:  return "Right Control";
		case sf::Keyboard::RShift:    return "Right Shift";
		case sf::Keyboard::RAlt:      return "Right Alt";
		case sf::Keyboard::RSystem:   return "Right System";
		case sf::Keyboard::Menu:      return "Menu";
		case sf::Keyboard::LBracket:  return "[";
		case sf::Keyboard::RBracket:  return "]";
		case sf::Keyboard::SemiColon: return ";";
		case sf::Keyboard::Comma:     return ",";
		case sf::Keyboard::Period:    return ".";
		case sf::Keyboard::Quote:     return "'";
		case sf::Keyboard::Slash:     return "/";
		case sf::Keyboard::BackSlash: return "\\";
		case sf::Keyboard::Tilde:     return "~";
		case sf::Keyboard::Equal:     return "=";
		case sf::Keyboard::Dash:      return "Dash";
		case sf::Keyboard::Space:     return "Space";
		case sf::Keyboard::Return:    return "Return";
		case sf::Keyboard::BackSpace: return "Backspace";
		case sf::Keyboard::Tab:       return "Tab";
		case sf::Keyboard::PageUp:    return "Page Up";
		case sf::Keyboard::PageDown:  return "Page Down";
		case sf::Keyboard::End:       return "End";
		case sf::Keyboard::Home:      return "Home";
		case sf::Keyboard::Insert:    return "Insert";
		case sf::Keyboard::Delete:    return "Delete";
		case sf::Keyboard::Add:       return "Add";
		case sf::Keyboard::Subtract:  return "Subtract";
		case sf::Keyboard::Multiply:  return "Multiply";
		case sf::Keyboard::Divide:    return "Divide";
		case sf::Keyboard::Left:      return "Left";
		case sf::Keyboard::Right:     return "Right";
		case sf::Keyboard::Up:        return "Up";
		case sf::Keyboard::Down:      return "Down";
		case sf::Keyboard::Numpad0:   return "Numpad 0";
		case sf::Keyboard::Numpad1:   return "Numpad 1";
		case sf::Keyboard::Numpad2:   return "Numpad 2";
		case sf::Keyboard::Numpad3:   return "Numpad 3";
		case sf::Keyboard::Numpad4:   return "Numpad 4";
		case sf::Keyboard::Numpad5:   return "Numpad 5";
		case sf::Keyboard::Numpad6:   return "Numpad 6";
		case sf::Keyboard::Numpad7:   return "Numpad 7";
		case sf::Keyboard::Numpad8:   return "Numpad 8";
		case sf::Keyboard::Numpad9:   return "Numpad 9";
		case sf::Keyboard::F1:        return "F1";
		case sf::Keyboard::F2:        return "F2";
		case sf::Keyboard::F3:        return "F3";
		case sf::Keyboard::F4:        return "F4";
		case sf::Keyboard::F5:        return "F5";
		case sf::Keyboard::F6:        return "F6";
		case sf::Keyboard::F7:        return "F7";
		case sf::Keyboard::F8:        return "F8";
		case sf::Keyboard::F9:        return "F9";
		case sf::Keyboard::F10:       return "F10";
		case sf::Keyboard::F11:       return "F11";
		case sf::Keyboard::F12:       return "F12";
		case sf::Keyboard::F13:       return "F13";
		case sf::Keyboard::F14:       return "F14";
		case sf::Keyboard::F15:       return "F15";
		case sf::Keyboard::Pause:     return "Pause";
		case sf::Keyboard::Unknown:   break;
		case sf::Keyboard::KeyCount:  break;
	}
	return "Invalid";
}


std::wstring Input::buttonToString(unsigned int button)
{
	if (button < sf::Joystick::ButtonCount)
	{
		std::wostringstream oss;
		oss << _t("menu.joystick.button") << L' ' << button;
		return oss.str();
	}
	return L"Invalid";
}


const std::wstring& Action::toString(Action::ID action)
{
	switch (action)
	{
		case Action::PAUSE:       return _t("action.pause");
		case Action::UP:          return _t("action.up");
		case Action::DOWN:        return _t("action.down");
		case Action::LEFT:        return _t("action.left");
		case Action::RIGHT:       return _t("action.right");
		case Action::USE_LASER:   return _t("action.laser");
		case Action::USE_COOLER:  return _t("action.cooler");
		case Action::USE_MISSILE: return _t("action.missile");
		default: break;
	}
	return _t("action.unknown");
}


std::istream& operator>>(std::istream& in, sf::Keyboard::Key& code)
{
	int i;
	in >> i;
	code = static_cast<sf::Keyboard::Key>(i);
	return in;
}
