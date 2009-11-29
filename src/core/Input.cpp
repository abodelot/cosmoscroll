#include "Input.hpp"
#include "Game.hpp"
#include "../utils/StringUtils.hpp"

#define JOY_ID			 0
#define JOY_DEADZONE	50.f


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
	SetKeyboardBind(sf::Key::P,        PAUSE);
	SetKeyboardBind(sf::Key::Space,    USE_WEAPON_1);
	SetKeyboardBind(sf::Key::A,        USE_WEAPON_2);
	SetKeyboardBind(sf::Key::LControl, USE_COOLER);
	SetKeyboardBind(sf::Key::Z,        USE_MISSILE);
	SetKeyboardBind(sf::Key::F1,       TAKE_SCREENSHOT);
	SetKeyboardBind(sf::Key::Escape,   EXIT_APP);

	// default joystick binding
	SetJoystickBind(1, PAUSE);
	SetJoystickBind(6, USE_WEAPON_1);
	SetJoystickBind(7, USE_WEAPON_2);
	SetJoystickBind(2, USE_COOLER);
	SetJoystickBind(3, USE_MISSILE);
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
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisY) < -JOY_DEADZONE;
		}
		if (action == Input::MOVE_DOWN)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisY) > JOY_DEADZONE;
		}
		if (action == Input::MOVE_LEFT)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisX) < -JOY_DEADZONE;
		}
		if (action == Input::MOVE_RIGHT)
		{
			return sfinput_->GetJoystickAxis(JOY_ID, sf::Joy::AxisX) > JOY_DEADZONE;
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
		puts("error: bad keyboard binding ignored");
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
		puts("error: bad joystick binding ignored");
	}
}


unsigned int Input::GetJoystickBind(Action action)
{
	return action < Input::COUNT ?
		action_to_joybutton_[action] : MAX_JOY_BUTTON;
}


const wchar_t* Input::ActionToString(Action action)
{
	switch (action)
	{
		case PAUSE:
			return L"Pause";
		case TAKE_SCREENSHOT:
			return L"Capture d'écran";
		case PANEL_UP:
			return L"Panneau en haut";
		case PANEL_DOWN:
			return L"Panneau en bas";
		case MOVE_UP:
			return L"Haut";
		case MOVE_DOWN:
			return L"Bas";
		case MOVE_LEFT:
			return L"Gauche";
		case MOVE_RIGHT:
			return L"Right";
		case USE_WEAPON_1:
			return L"Arme 1";
		case USE_WEAPON_2:
			return L"Arme 2";
		case USE_COOLER:
			return L"Utiliser bonus Glaçon";
		case USE_MISSILE:
			return L"Utiliser bonus Missile";
		case ENTER:
			return L"Valider";
		case EXIT_APP:
			return L"Quitter le jeu";
		default:
			break;
	}
	return L"<Unknown action>";
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
		case sf::Key::Num0: return "Num0";
		case sf::Key::Num1: return "Num1";
		case sf::Key::Num2: return "Num2";
		case sf::Key::Num3: return "Num3";
		case sf::Key::Num4: return "Num4";
		case sf::Key::Num5: return "Num5";
		case sf::Key::Num6: return "Num6";
		case sf::Key::Num7: return "Num7";
		case sf::Key::Num8: return "Num8";
		case sf::Key::Num9: return "Num9";
		case sf::Key::Escape: return "Escape";
		case sf::Key::LControl: return "LControl";
		case sf::Key::LShift: return "LShift";
		case sf::Key::LAlt: return "LAlt";
		case sf::Key::LSystem: return "LSystem";
		case sf::Key::RControl: return "RControl";
		case sf::Key::RShift: return "RShift";
		case sf::Key::RAlt: return "RAlt";
		case sf::Key::RSystem: return "RSystem";
		case sf::Key::Menu: return "Menu";
		case sf::Key::LBracket: return "LBracket";
		case sf::Key::RBracket: return "RBracket";
		case sf::Key::SemiColon: return "SemiColon";
		case sf::Key::Comma: return "Comma";
		case sf::Key::Period: return "Period";
		case sf::Key::Quote: return "Quote";
		case sf::Key::Slash: return "Slash";
		case sf::Key::BackSlash: return "BackSlash";
		case sf::Key::Tilde: return "Tilde";
		case sf::Key::Equal: return "Equal";
		case sf::Key::Dash: return "Dash";
		case sf::Key::Space: return "Space";
		case sf::Key::Return: return "Return";
		case sf::Key::Back: return "Back";
		case sf::Key::Tab: return "Tab";
		case sf::Key::PageUp: return "PageUp";
		case sf::Key::PageDown: return "PageDown";
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
		case sf::Key::Numpad0: return "Numpad0";
		case sf::Key::Numpad1: return "Numpad1";
		case sf::Key::Numpad2: return "Numpad2";
		case sf::Key::Numpad3: return "Numpad3";
		case sf::Key::Numpad4: return "Numpad4";
		case sf::Key::Numpad5: return "Numpad5";
		case sf::Key::Numpad6: return "Numpad6";
		case sf::Key::Numpad7: return "Numpad7";
		case sf::Key::Numpad8: return "Numpad8";
		case sf::Key::Numpad9: return "Numpad9";
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
	std::wstring str;
	if (device == Input::KEYBOARD)
	{
		str = L"Appuyez sur une touche du clavier\npour l'action '";
	}
	else if (device == Input::JOYSTICK)
	{
		str = L"Appuyez sur un bouton du contrôleur\npour l'action '";
	}
	str += Input::ActionToString(action);
	str += L"'\n\n(Échap pour annuler)";

	sf::String prompt;
	prompt.SetText(str);
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


void Input::LoadFromConfig(ConfigParser& config)
{
	config.SeekSection("Keyboard");
	sf::Key::Code key;
	if (config.ReadItem("move_up", key))
		SetKeyboardBind(key, MOVE_UP);
	if (config.ReadItem("move_down", key))
		SetKeyboardBind(key, MOVE_DOWN);
	if (config.ReadItem("move_left", key))
		SetKeyboardBind(key, MOVE_LEFT);
	if (config.ReadItem("move_right", key))
		SetKeyboardBind(key, MOVE_RIGHT);
	if (config.ReadItem("pause", key))
		SetKeyboardBind(key, PAUSE);
	if (config.ReadItem("weapon_1", key))
		SetKeyboardBind(key, USE_WEAPON_1);
	if (config.ReadItem("weapon_2", key))
		SetKeyboardBind(key, USE_WEAPON_2);
	if (config.ReadItem("use_cooler", key))
		SetKeyboardBind(key, USE_COOLER);
	if (config.ReadItem("use_missile", key))
		SetKeyboardBind(key, USE_MISSILE);

	config.SeekSection("Joystick");
	unsigned int button;
	if (config.ReadItem("pause", button))
		SetJoystickBind(button, PAUSE);
	if (config.ReadItem("weapon_1", button))
		SetJoystickBind(button, USE_WEAPON_1);
	if (config.ReadItem("weapon_2", button))
		SetJoystickBind(button, USE_WEAPON_2);
	if (config.ReadItem("use_cooler", button))
		SetJoystickBind(button, USE_COOLER);
	if (config.ReadItem("use_missile", button))
		SetJoystickBind(button, USE_MISSILE);
}


void Input::SaveToConfig(ConfigParser& config) const
{
	config.SeekSection("Keyboard");
	config.WriteItem("move_up",     action_to_key_[MOVE_UP]);
	config.WriteItem("move_down",   action_to_key_[MOVE_DOWN]);
	config.WriteItem("move_left",   action_to_key_[MOVE_LEFT]);
	config.WriteItem("move_right",  action_to_key_[MOVE_RIGHT]);
	config.WriteItem("pause",       action_to_key_[PAUSE]);
	config.WriteItem("weapon_1",    action_to_key_[USE_WEAPON_1]);
	config.WriteItem("weapon_2",    action_to_key_[USE_WEAPON_2]);
	config.WriteItem("use_cooler",  action_to_key_[USE_COOLER]);
	config.WriteItem("use_missile", action_to_key_[USE_MISSILE]);

	config.SeekSection("Joystick");
	config.WriteItem("pause",       action_to_joybutton_[PAUSE]);
	config.WriteItem("weapon_1",    action_to_joybutton_[USE_WEAPON_1]);
	config.WriteItem("weapon_2",    action_to_joybutton_[USE_WEAPON_2]);
	config.WriteItem("use_cooler",  action_to_joybutton_[USE_COOLER]);
	config.WriteItem("use_missile", action_to_joybutton_[USE_MISSILE]);
}


std::istream& operator>>(std::istream& in, sf::Key::Code& code)
{
	int i;
	in >> i;
	code = static_cast<sf::Key::Code>(i);
	return in;
}

