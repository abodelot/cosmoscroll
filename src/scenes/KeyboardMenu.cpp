#include "KeyboardMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/I18n.hpp"


static void build_action_string(std::wstring& label, Input::Action action)
{
	Input& input = Input::GetInstance();

	label = Input::ActionToString(action);
	label += L" : ";
	// convert key string to wstring
	const std::string str = Input::KeyToString(input.GetKeyboardBind(action));
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	label += wstr;
}


KeyboardMenu::KeyboardMenu()
{
	SetTitle(I18n::t("menu.keyboard.title"));
	SetOffsetY(100);
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

	AddOption(I18n::t("menu.back"), 8000);
}


void KeyboardMenu::AddBindOpt(Input::Action action)
{
	std::wstring label;
	build_action_string(label, action);
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
		Input::Action action = (Input::Action) id;
		Input::GetInstance().AskUserInput(Input::KEYBOARD, action);
		std::wstring label;
		build_action_string(label, action);
		SetTextOption(label, id);
	}
}
