#include <cstdio>
#include <SFML/Graphics/String.hpp>
#include "Input.hpp"
#include "Game.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"
#include "utils/IniParser.hpp"

#define JOY_ID			 0


Input& Input::GetInstance()
{
	static Input self;
	return self;
}


void Input::Init(const sf::Input& sfinput)
{
	sfinput_ = &sfinput;
	device_flag_ = ALL;

	// unset bindings for everything
	for (int i = 0; i < Input::COUNT; ++i)
	{
		action_to_key_[i] = sf::Key::Count;
		action_to_joybutton_[i] = MAX_JOY_BUTTON;
	}

	for (int i = 0; i < sf::Key::Count; ++i)
	{
		key_to_action_[i] = Input::COUNT;
	}

	for (int i = 0; i < MAX_JOY_BUTTON; ++i)
	{
		joybutton_to_action_[i] = Input::COUNT;
	}

	// default keyboard binding
	SetKeyboardBind(sf::Key::Return,   ENTER);
	SetKeyboardBind(sf::Key::Up,       MOVE_UP);
	SetKeyboardBind(sf::Key::Down,     MOVE_DOWN);
	SetKeyboardBind(sf::Key::Left,     MOVE_LEFT);
	SetKeyboardBind(sf::Key::Right,    MOVE_RIGHT);
	SetKeyboardBind(sf::Key::PageUp,   PANEL_UP);
	SetKeyboardBind(sf::Key::PageDown, PANEL_DOWN);
	SetKeyboardBind(sf::Key::Escape,   PAUSE);
	SetKeyboardBind(sf::Key::Space,    USE_LASER);
	SetKeyboardBind(sf::Key::A,        USE_PLASMA);
	SetKeyboardBind(sf::Key::LControl, USE_COOLER);
	SetKeyboardBind(sf::Key::Z,        USE_MISSILE);
	SetKeyboardBind(sf::Key::F1,       TAKE_SCREENSHOT);

	// default joystick binding
	SetJoystickBind(0, ENTER);
	SetJoystickBind(1, PAUSE);
	SetJoystickBind(6, USE_LASER);
	SetJoystickBind(7, USE_PLASMA);
	SetJoystickBind(2, USE_COOLER);
	SetJoystickBind(3, USE_MISSILE);

	sensitivity_ = 50;
}


Input::Action Input::EventToAction(const sf::Event& event)
{
	switch (event.Type)
	{
		case sf::Event::KeyPressed:
			return (device_flag_ & Input::KEYBOARD ) ?
				key_to_action_[event.Key.Code] : Input::COUNT;

		case sf::Event::JoyButtonPressed:
			return (device_flag_ & Input::JOYSTICK) ?
				joybutton_to_action_[event.JoyButton.Button] : Input::COUNT;

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
	if ((device_flag_ & KEYBOARD) && sfinput_->IsKeyDown(action_to_key_[action]))
	{
		return true;
	}
	if (device_flag_ & JOYSTICK)
	{
		if (sfinput_->IsJoystickButtonDown(JOY_ID, action_to_joybutton_[action]))
		{
			return true;
		}
		if (action == Input::MOVE_UP)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisY) < -sensitivity_;
		}
		if (action == Input::MOVE_DOWN)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisY) > sensitivity_;
		}
		if (action == Input::MOVE_LEFT)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisX) < -sensitivity_;
		}
		if (action == Input::MOVE_RIGHT)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisX) > sensitivity_;
		}
	}
	return false;
}


void Input::SetKeyboardBind(sf::Key::Code key, Action action)
{
	if (action < Input::COUNT && key < sf::Key::Count)
	{
		sf::Key::Code old_key = action_to_key_[action];
		if (old_key != sf::Key::Count)
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


sf::Key::Code Input::GetKeyboardBind(Action action)
{
	return action < Input::COUNT ?
		action_to_key_[action] : sf::Key::Count;
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
		case USE_PLASMA:
			return _t("action.plasma");
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
		case sf::Key::A: return "A";
		case sf::Key::B: return "B";
		case sf::Key::C: return "C";
		case sf::Key::D: return "D";
		case sf::Key::E: return "E";
		case sf::Key::F: return "F";
		case sf::Key::G: return "G";
		case sf::Key::H: return "H";
		case sf::Key::I: return "I";
		case sf::Key::J: return "J";
		case sf::Key::K: return "K";
		case sf::Key::L: return "L";
		case sf::Key::M: return "M";
		case sf::Key::N: return "N";
		case sf::Key::O: return "O";
		case sf::Key::P: return "P";
		case sf::Key::Q: return "Q";
		case sf::Key::R: return "R";
		case sf::Key::S: return "S";
		case sf::Key::T: return "T";
		case sf::Key::U: return "U";
		case sf::Key::V: return "V";
		case sf::Key::W: return "W";
		case sf::Key::X: return "X";
		case sf::Key::Y: return "Y";
		case sf::Key::Z: return "Z";
		case sf::Key::Num0: return "Num 0";
		case sf::Key::Num1: return "Num 1";
		case sf::Key::Num2: return "Num 2";
		case sf::Key::Num3: return "Num 3";
		case sf::Key::Num4: return "Num 4";
		case sf::Key::Num5: return "Num 5";
		case sf::Key::Num6: return "Num 6";
		case sf::Key::Num7: return "Num 7";
		case sf::Key::Num8: return "Num 8";
		case sf::Key::Num9: return "Num 9";
		case sf::Key::Escape: return "Escape";
		case sf::Key::LControl: return "Left Control";
		case sf::Key::LShift: return "Left Shift";
		case sf::Key::LAlt: return "Left Alt";
		case sf::Key::LSystem: return "Left System";
		case sf::Key::RControl: return "Right Control";
		case sf::Key::RShift: return "Right Shift";
		case sf::Key::RAlt: return "RAlt";
		case sf::Key::RSystem: return "Right System";
		case sf::Key::Menu: return "Menu";
		case sf::Key::LBracket: return "Left Bracket";
		case sf::Key::RBracket: return "Right Bracket";
		case sf::Key::SemiColon: return "Semi Colon";
		case sf::Key::Comma: return "Comma";
		case sf::Key::Period: return "Period";
		case sf::Key::Quote: return "Quote";
		case sf::Key::Slash: return "Slash";
		case sf::Key::BackSlash: return "Back Slash";
		case sf::Key::Tilde: return "Tilde";
		case sf::Key::Equal: return "Equal";
		case sf::Key::Dash: return "Dash";
		case sf::Key::Space: return "Space";
		case sf::Key::Return: return "Return";
		case sf::Key::Back: return "Back";
		case sf::Key::Tab: return "Tab";
		case sf::Key::PageUp: return "Page Up";
		case sf::Key::PageDown: return "Page Down";
		case sf::Key::End: return "End";
		case sf::Key::Home: return "Home";
		case sf::Key::Insert: return "Insert";
		case sf::Key::Delete: return "Delete";
		case sf::Key::Add: return "Add";
		case sf::Key::Subtract: return "Subtract";
		case sf::Key::Multiply: return "Multiply";
		case sf::Key::Divide: return "Divide";
		case sf::Key::Left: return "Left";
		case sf::Key::Right: return "Right";
		case sf::Key::Up: return "Up";
		case sf::Key::Down: return "Down";
		case sf::Key::Numpad0: return "Numpad 0";
		case sf::Key::Numpad1: return "Numpad 1";
		case sf::Key::Numpad2: return "Numpad 2";
		case sf::Key::Numpad3: return "Numpad 3";
		case sf::Key::Numpad4: return "Numpad 4";
		case sf::Key::Numpad5: return "Numpad 5";
		case sf::Key::Numpad6: return "Numpad 6";
		case sf::Key::Numpad7: return "Numpad 7";
		case sf::Key::Numpad8: return "Numpad 8";
		case sf::Key::Numpad9: return "Numpad 9";
		case sf::Key::F1: return "F1";
		case sf::Key::F2: return "F2";
		case sf::Key::F3: return "F3";
		case sf::Key::F4: return "F4";
		case sf::Key::F5: return "F5";
		case sf::Key::F6: return "F6";
		case sf::Key::F7: return "F7";
		case sf::Key::F8: return "F8";
		case sf::Key::F9: return "F9";
		case sf::Key::F10: return "F10";
		case sf::Key::F11: return "F11";
		case sf::Key::F12: return "F12";
		case sf::Key::F13: return "F13";
		case sf::Key::F14: return "F14";
		case sf::Key::F15: return "F15";
		case sf::Key::Pause: return "Pause";
	}
	return "<Unknown key>";
}


void Input::SetDevices(unsigned int flag)
{
	device_flag_ = flag;
}


void Input::AskUserInput(Device device, Action action)
{
	std::wstring s;
	if (device == Input::KEYBOARD)
	{
		s = _t("input.press_key");
	}
	else if (device == Input::JOYSTICK)
	{
		s = _t("input.press_button");
	}
	s += L" '" + Input::ActionToString(action) + L"'\n\n" + _t("input.cancel");

	sf::String prompt;
	prompt.SetText(s);
	prompt.SetColor(sf::Color::White);
	sf::FloatRect rect = prompt.GetRect();
	prompt.SetPosition(
		(Game::WIDTH - rect.GetWidth()) / 2,
		(Game::HEIGHT - rect.GetHeight()) / 2
	);

	sf::Event event;
	bool running = true;
	sf::RenderWindow& app = Game::GetInstance().GetApp();
	while (running)
	{
		while (app.GetEvent(event))
		{
			if (event.Type == sf::Event::KeyPressed)
			{
				if (event.Key.Code == sf::Key::Escape)
				{
					running = false;
				}
				else if (device == Input::KEYBOARD)
				{
					running = false;
					SetKeyboardBind(event.Key.Code, action);
				}
			}
			else if (event.Type == sf::Event::JoyButtonPressed
				&& device == Input::JOYSTICK)
			{
				running = false;
				SetJoystickBind(event.JoyButton.Button, action);
			}
		}
		app.Clear();
		app.Draw(prompt);
		app.Display();
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
	sf::Key::Code key;
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
	if (config.Get("plasma", key))
		SetKeyboardBind(key, USE_PLASMA);
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
	if (config.Get("plasma", button))
		SetJoystickBind(button, USE_PLASMA);
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
	config.Set("plasma",      action_to_key_[USE_PLASMA]);
	config.Set("use_cooler",  action_to_key_[USE_COOLER]);
	config.Set("use_missile", action_to_key_[USE_MISSILE]);

	config.SeekSection("Joystick");
	config.Set("pause",       action_to_joybutton_[PAUSE]);
	config.Set("laser",       action_to_joybutton_[USE_LASER]);
	config.Set("plasma",      action_to_joybutton_[USE_PLASMA]);
	config.Set("use_cooler",  action_to_joybutton_[USE_COOLER]);
	config.Set("use_missile", action_to_joybutton_[USE_MISSILE]);
	config.Set("sensitivity", sensitivity_);
}


std::istream& operator>>(std::istream& in, sf::Key::Code& code)
{
	int i;
	in >> i;
	code = static_cast<sf::Key::Code>(i);
	return in;
}

