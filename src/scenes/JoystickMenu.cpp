#include "JoystickMenu.hpp"
#include "../core/Game.hpp"
#include "../core/Input.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"


JoystickMenu::JoystickMenu()
{
	SetTitle(I18n::t("menu.joystick.title"));
	SetOffsetY(100);
}


void JoystickMenu::Poke()
{
	Clear();
	AddBindOpt(Input::USE_WEAPON_1);
	AddBindOpt(Input::USE_WEAPON_2);
	AddBindOpt(Input::USE_COOLER);
	AddBindOpt(Input::USE_MISSILE);
	AddBindOpt(Input::PAUSE);

	AddOption(I18n::t("menu.back"), -1);
}


void JoystickMenu::AddBindOpt(Input::Action action)
{
	Input& input = Input::GetInstance();

	std::wstring label = str_sprintf(I18n::t("menu.joystick.button_bind").c_str(),
		Input::ActionToString(action), input.GetJoystickBind(action));

	AddOption(label, action);
}


void JoystickMenu::Callback(int id)
{
	if (id == -1)
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
