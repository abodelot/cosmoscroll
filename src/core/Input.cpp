#include <cstdio>
#include <SFML/Graphics.hpp>
#include "Input.hpp"
#include "Game.hpp"
#include "Resources.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "utils/IniParser.hpp"

#define JOY_ID			 0


Input& Input::GetInstance()
{
	static Input self;
	return self;
}


void Input::Init()
{
	device_flag_ = ALL;

	// unset bindings for everything
	for (int i = 0; i < Input::COUNT; ++i)
	{
		action_to_key_[i] = sf::Keyboard::KeyCount;
		action_to_joybutton_[i] = MAX_JOY_BUTTON;
	}

	for (int i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		key_to_action_[i] = Input::COUNT;
	}

	for (int i = 0; i < MAX_JOY_BUTTON; ++i)
	{
		joybutton_to_action_[i] = Input::COUNT;
	}

	// default keyboard binding
	SetKeyboardBind(sf::Keyboard::Return,   ENTER);
	SetKeyboardBind(sf::Keyboard::Up,       MOVE_UP);
	SetKeyboardBind(sf::Keyboard::Down,     MOVE_DOWN);
	SetKeyboardBind(sf::Keyboard::Left,     MOVE_LEFT);
	SetKeyboardBind(sf::Keyboard::Right,    MOVE_RIGHT);
	SetKeyboardBind(sf::Keyboard::PageUp,   PANEL_UP);
	SetKeyboardBind(sf::Keyboard::PageDown, PANEL_DOWN);
	SetKeyboardBind(sf::Keyboard::Escape,   PAUSE);
	SetKeyboardBind(sf::Keyboard::Space,    USE_LASER);
	SetKeyboardBind(sf::Keyboard::LControl, USE_COOLER);
	SetKeyboardBind(sf::Keyboard::A,        USE_MISSILE);
	SetKeyboardBind(sf::Keyboard::F1,       TAKE_SCREENSHOT);

	// default joystick binding
	SetJoystickBind(0, ENTER);
	SetJoystickBind(1, PAUSE);
	SetJoystickBind(6, USE_LASER);
	SetJoystickBind(2, USE_COOLER);
	SetJoystickBind(3, USE_MISSILE);

	sensitivity_ = 50;
}


Input::Action Input::EventToAction(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			return (device_flag_ & Input::KEYBOARD ) ?
				key_to_action_[event.key.code] : Input::COUNT;

		case sf::Event::JoystickButtonPressed:
			return (device_flag_ & Input::JOYSTICK) ?
				joybutton_to_action_[event.joystickButton.button] : Input::COUNT;

		case sf::Event::Closed:
			return EXIT_APP;

		case sf::Event::LostFocus:
			return PAUSE;

		default:
			break;
	}
	return COUNT;
}


bool Input::HasInput(Action action)
{
	if ((device_flag_ & KEYBOARD) && sf::Keyboard::isKeyPressed(action_to_key_[action]))
	{
		return true;
	}
	if (device_flag_ & JOYSTICK)
	{
		if (sf::Joystick::isButtonPressed(JOY_ID, action_to_joybutton_[action]))
		{
			return true;
		}
		if (action == Input::MOVE_UP)
		{
			return sf::Joystick::getAxisPosition(JOY_ID, sf::Joystick::Y) < -sensitivity_;
		}
		if (action == Input::MOVE_DOWN)
		{
			return sf::Joystick::getAxisPosition(JOY_ID, sf::Joystick::Y) > sensitivity_;
		}
		if (action == Input::MOVE_LEFT)
		{
			return sf::Joystick::getAxisPosition(JOY_ID, sf::Joystick::X) < -sensitivity_;
		}
		if (action == Input::MOVE_RIGHT)
		{
			return sf::Joystick::getAxisPosition(JOY_ID, sf::Joystick::X) > sensitivity_;
		}
	}
	return false;
}


void Input::SetKeyboardBind(sf::Keyboard::Key key, Action action)
{
	if (action < Input::COUNT && key < sf::Keyboard::KeyCount)
	{
		sf::Keyboard::Key old_key = action_to_key_[action];
		if (old_key != sf::Keyboard::KeyCount)
		{
			key_to_action_[old_key] = COUNT;
		}
		action_to_key_[action] = key;
		key_to_action_[key] = action;
	}
	else
	{
		fprintf(stderr, "error: bad keyboard binding ignored\n");
	}
}


sf::Keyboard::Key Input::GetKeyboardBind(Action action)
{
	return action < Input::COUNT ?
		action_to_key_[action] : sf::Keyboard::KeyCount;
}


void Input::SetJoystickBind(unsigned int joybutton, Action action)
{
	if (action < Input::COUNT && joybutton < MAX_JOY_BUTTON)
	{
		action_to_joybutton_[action] = joybutton;
		joybutton_to_action_[joybutton] = action;
	}
	else
	{
		fprintf(stderr, "error: bad joystick binding ignored\n");
	}
}


unsigned int Input::GetJoystickBind(Action action)
{
	return action < Input::COUNT ?
		action_to_joybutton_[action] : MAX_JOY_BUTTON;
}


const std::wstring& Input::ActionToString(Action action)
{
	switch (action)
	{
		case PAUSE:
			return _t("action.pause");
		case MOVE_UP:
			return _t("action.up");
		case MOVE_DOWN:
			return _t("action.down");
		case MOVE_LEFT:
			return _t("action.left");
		case MOVE_RIGHT:
			return _t("action.right");
		case USE_LASER:
			return _t("action.laser");
		case USE_COOLER:
			return _t("action.cooler");
		case USE_MISSILE:
			return _t("action.missile");
		default:
			break;
	}
	return _t("action.unknown");
}


const char* Input::KeyToString(int key)
{
	switch (key)
	{
		case sf::Keyboard::A: return "A";
		case sf::Keyboard::B: return "B";
		case sf::Keyboard::C: return "C";
		case sf::Keyboard::D: return "D";
		case sf::Keyboard::E: return "E";
		case sf::Keyboard::F: return "F";
		case sf::Keyboard::G: return "G";
		case sf::Keyboard::H: return "H";
		case sf::Keyboard::I: return "I";
		case sf::Keyboard::J: return "J";
		case sf::Keyboard::K: return "K";
		case sf::Keyboard::L: return "L";
		case sf::Keyboard::M: return "M";
		case sf::Keyboard::N: return "N";
		case sf::Keyboard::O: return "O";
		case sf::Keyboard::P: return "P";
		case sf::Keyboard::Q: return "Q";
		case sf::Keyboard::R: return "R";
		case sf::Keyboard::S: return "S";
		case sf::Keyboard::T: return "T";
		case sf::Keyboard::U: return "U";
		case sf::Keyboard::V: return "V";
		case sf::Keyboard::W: return "W";
		case sf::Keyboard::X: return "X";
		case sf::Keyboard::Y: return "Y";
		case sf::Keyboard::Z: return "Z";
		case sf::Keyboard::Num0: return "Num 0";
		case sf::Keyboard::Num1: return "Num 1";
		case sf::Keyboard::Num2: return "Num 2";
		case sf::Keyboard::Num3: return "Num 3";
		case sf::Keyboard::Num4: return "Num 4";
		case sf::Keyboard::Num5: return "Num 5";
		case sf::Keyboard::Num6: return "Num 6";
		case sf::Keyboard::Num7: return "Num 7";
		case sf::Keyboard::Num8: return "Num 8";
		case sf::Keyboard::Num9: return "Num 9";
		case sf::Keyboard::Escape: return "Escape";
		case sf::Keyboard::LControl: return "Left Control";
		case sf::Keyboard::LShift: return "Left Shift";
		case sf::Keyboard::LAlt: return "Left Alt";
		case sf::Keyboard::LSystem: return "Left System";
		case sf::Keyboard::RControl: return "Right Control";
		case sf::Keyboard::RShift: return "Right Shift";
		case sf::Keyboard::RAlt: return "RAlt";
		case sf::Keyboard::RSystem: return "Right System";
		case sf::Keyboard::Menu: return "Menu";
		case sf::Keyboard::LBracket: return "Left Bracket";
		case sf::Keyboard::RBracket: return "Right Bracket";
		case sf::Keyboard::SemiColon: return "Semi Colon";
		case sf::Keyboard::Comma: return "Comma";
		case sf::Keyboard::Period: return "Period";
		case sf::Keyboard::Quote: return "Quote";
		case sf::Keyboard::Slash: return "Slash";
		case sf::Keyboard::BackSlash: return "Back Slash";
		case sf::Keyboard::Tilde: return "Tilde";
		case sf::Keyboard::Equal: return "Equal";
		case sf::Keyboard::Dash: return "Dash";
		case sf::Keyboard::Space: return "Space";
		case sf::Keyboard::Return: return "Return";
		case sf::Keyboard::BackSpace: return "Backspace";
		case sf::Keyboard::Tab: return "Tab";
		case sf::Keyboard::PageUp: return "Page Up";
		case sf::Keyboard::PageDown: return "Page Down";
		case sf::Keyboard::End: return "End";
		case sf::Keyboard::Home: return "Home";
		case sf::Keyboard::Insert: return "Insert";
		case sf::Keyboard::Delete: return "Delete";
		case sf::Keyboard::Add: return "Add";
		case sf::Keyboard::Subtract: return "Subtract";
		case sf::Keyboard::Multiply: return "Multiply";
		case sf::Keyboard::Divide: return "Divide";
		case sf::Keyboard::Left: return "Left";
		case sf::Keyboard::Right: return "Right";
		case sf::Keyboard::Up: return "Up";
		case sf::Keyboard::Down: return "Down";
		case sf::Keyboard::Numpad0: return "Numpad 0";
		case sf::Keyboard::Numpad1: return "Numpad 1";
		case sf::Keyboard::Numpad2: return "Numpad 2";
		case sf::Keyboard::Numpad3: return "Numpad 3";
		case sf::Keyboard::Numpad4: return "Numpad 4";
		case sf::Keyboard::Numpad5: return "Numpad 5";
		case sf::Keyboard::Numpad6: return "Numpad 6";
		case sf::Keyboard::Numpad7: return "Numpad 7";
		case sf::Keyboard::Numpad8: return "Numpad 8";
		case sf::Keyboard::Numpad9: return "Numpad 9";
		case sf::Keyboard::F1: return "F1";
		case sf::Keyboard::F2: return "F2";
		case sf::Keyboard::F3: return "F3";
		case sf::Keyboard::F4: return "F4";
		case sf::Keyboard::F5: return "F5";
		case sf::Keyboard::F6: return "F6";
		case sf::Keyboard::F7: return "F7";
		case sf::Keyboard::F8: return "F8";
		case sf::Keyboard::F9: return "F9";
		case sf::Keyboard::F10: return "F10";
		case sf::Keyboard::F11: return "F11";
		case sf::Keyboard::F12: return "F12";
		case sf::Keyboard::F13: return "F13";
		case sf::Keyboard::F14: return "F14";
		case sf::Keyboard::F15: return "F15";
		case sf::Keyboard::Pause: return "Pause";
	}
	return "<Unknown key>";
}


void Input::SetDevices(unsigned int flag)
{
	device_flag_ = flag;
}


void Input::AskUserInput(Device device, Action action)
{
	sf::String s;
	if (device == Input::KEYBOARD)
	{
		s = _t("input.press_key");
	}
	else if (device == Input::JOYSTICK)
	{
		s = _t("input.press_button");
	}
	s += L" '" + Input::ActionToString(action) + L"'\n\n" + _t("input.cancel");

	sf::Text prompt;
	prompt.setString(s);
	prompt.setColor(sf::Color::White);
	prompt.setFont(Resources::getFont("Ubuntu-R.ttf"));
	sf::FloatRect rect = prompt.getLocalBounds();
	prompt.setPosition(
		(int) (Game::WIDTH - rect.width) / 2,
		(int) (Game::HEIGHT - rect.height) / 2
	);

	sf::Event event;
	bool running = true;
	sf::RenderWindow& app = Game::GetInstance().GetApp();
	while (running)
	{
		while (app.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					running = false;
				}
				else if (device == Input::KEYBOARD)
				{
					running = false;
					SetKeyboardBind(event.key.code, action);
				}
			}
			else if (event.type == sf::Event::JoystickButtonPressed
				&& device == Input::JOYSTICK)
			{
				running = false;
				SetJoystickBind(event.joystickButton.button, action);
			}
		}
		app.clear();
		app.draw(prompt);
		app.display();
	}
}


int Input::GetSensitivity() const
{
	return sensitivity_;
}


void Input::SetSensitivity(int sensitivity)
{
	if (sensitivity < 0)
	{
		sensitivity = 0;
	}
	else if (sensitivity > 99)
	{
		sensitivity = 99;
	}
	sensitivity_ = sensitivity;
}


void Input::LoadFromConfig(IniParser& config)
{
	config.SeekSection("Keyboard");
	sf::Keyboard::Key key;
	if (config.Get("move_up", key))
		SetKeyboardBind(key, MOVE_UP);
	if (config.Get("move_down", key))
		SetKeyboardBind(key, MOVE_DOWN);
	if (config.Get("move_left", key))
		SetKeyboardBind(key, MOVE_LEFT);
	if (config.Get("move_right", key))
		SetKeyboardBind(key, MOVE_RIGHT);
	if (config.Get("laser", key))
		SetKeyboardBind(key, USE_LASER);
	if (config.Get("use_cooler", key))
		SetKeyboardBind(key, USE_COOLER);
	if (config.Get("use_missile", key))
		SetKeyboardBind(key, USE_MISSILE);

	config.SeekSection("Joystick");
	unsigned int button;
	if (config.Get("pause", button))
		SetJoystickBind(button, PAUSE);
	if (config.Get("laser", button))
		SetJoystickBind(button, USE_LASER);
	if (config.Get("use_cooler", button))
		SetJoystickBind(button, USE_COOLER);
	if (config.Get("use_missile", button))
		SetJoystickBind(button, USE_MISSILE);

	config.Get("sensitivity", sensitivity_);
}


void Input::SaveToConfig(IniParser& config) const
{
	config.SeekSection("Keyboard");
	config.Set("move_up",     action_to_key_[MOVE_UP]);
	config.Set("move_down",   action_to_key_[MOVE_DOWN]);
	config.Set("move_left",   action_to_key_[MOVE_LEFT]);
	config.Set("move_right",  action_to_key_[MOVE_RIGHT]);
	config.Set("laser",       action_to_key_[USE_LASER]);
	config.Set("use_cooler",  action_to_key_[USE_COOLER]);
	config.Set("use_missile", action_to_key_[USE_MISSILE]);

	config.SeekSection("Joystick");
	config.Set("pause",       action_to_joybutton_[PAUSE]);
	config.Set("laser",       action_to_joybutton_[USE_LASER]);
	config.Set("use_cooler",  action_to_joybutton_[USE_COOLER]);
	config.Set("use_missile", action_to_joybutton_[USE_MISSILE]);
	config.Set("sensitivity", sensitivity_);
}


std::istream& operator>>(std::istream& in, sf::Keyboard::Key& code)
{
	int i;
	in >> i;
	code = static_cast<sf::Keyboard::Key>(i);
	return in;
}

