#include "KeyboardMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/StringUtils.hpp"


KeyboardMenu::KeyboardMenu()
{
	SetTitle(L"Configuration clavier");
	SetOffset(Game::MARGIN_X, 100);
	SetTextSize(26);
}


void KeyboardMenu::Poke()
{
	Clear();
	AddBindOpt(Input::MOVE_UP);
	AddBindOpt(Input::MOVE_DOWN);
	AddBindOpt(Input::MOVE_LEFT);
	AddBindOpt(Input::MOVE_RIGHT);
	AddBindOpt(Input::USE_WEAPON_1);
	AddBindOpt(Input::USE_WEAPON_2);
	AddBindOpt(Input::USE_COOLER);
	AddBindOpt(Input::USE_MISSILE);
	AddBindOpt(Input::PAUSE);

	AddOption("Retour", 8000);
}


void KeyboardMenu::AddBindOpt(Input::Action action)
{
	Input& input = Input::GetInstance();

	std::wstring label = Input::ActionToString(action);
	label += str_sprintf(L" : %s", Input::KeyToString(input.GetKeyboardBind(action)));

	AddOption(label, action);
}


void KeyboardMenu::Callback(int id)
{
	if (id == 8000)
	{
		Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
	}
	else
	{
		// Input::Action enumerations are used as menu ids
		Input& input = Input::GetInstance();
		Input::Action action = (Input::Action) id;
		input.AskUserInput(Input::KEYBOARD, action);
		std::wstring label = Input::ActionToString(action);
		label += str_sprintf(L" : %s", Input::KeyToString(input.GetKeyboardBind(action)));
		SetTextOption(label, id);
	}
}
