#include "JoystickMenu.hpp"
#include "../core/Game.hpp"
#include "../core/Input.hpp"
#include "../utils/StringUtils.hpp"


JoystickMenu::JoystickMenu()
{
	SetTitle(L"Configuration joystick");
	SetOffset(Game::MARGIN_X, 100);
}


void JoystickMenu::Poke()
{
	Clear();
	AddBindOpt(Input::USE_WEAPON_1);
	AddBindOpt(Input::USE_WEAPON_2);
	AddBindOpt(Input::USE_COOLER);
	AddBindOpt(Input::USE_MISSILE);
	AddBindOpt(Input::PAUSE);

	AddOption("Retour", 0);
}


void JoystickMenu::AddBindOpt(Input::Action action)
{
	Input& input = Input::GetInstance();

	std::wstring label = Input::ActionToString(action);
	label += str_sprintf(L" : bouton %d", input.GetJoystickBind(action));

	AddOption(label, action);
}


void JoystickMenu::Callback(int id)
{
	if (id == 0)
	{
		Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
	}
	else
	{
		// Input::Action enumerations are used as menu ids
		Input& input = Input::GetInstance();
		input.AskUserInput(Input::JOYSTICK, (Input::Action) id);
		Poke();
	}
}
