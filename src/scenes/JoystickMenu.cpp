#include "JoystickMenu.hpp"
#include "../core/Game.hpp"
#include "../core/Input.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"


JoystickMenu::JoystickMenu()
{
	SetTitle(I18n::t("menu.joystick.title"));

	but_weapon1_ = new gui::Button(this, GetButtonLabel(Input::USE_WEAPON_1), 210, 120);
	but_weapon1_->SetCallbackID(Input::USE_WEAPON_1);
	but_weapon2_ = new gui::Button(this, GetButtonLabel(Input::USE_WEAPON_2), 210, 160);
	but_weapon2_->SetCallbackID(Input::USE_WEAPON_2);
	but_missile_ = new gui::Button(this, GetButtonLabel(Input::USE_MISSILE), 210, 200);
	but_missile_->SetCallbackID(Input::USE_MISSILE);
	but_cooler_ =  new gui::Button(this, GetButtonLabel(Input::USE_COOLER), 210, 240);
	but_cooler_->SetCallbackID(Input::USE_COOLER);
	but_pause_ =   new gui::Button(this, GetButtonLabel(Input::PAUSE), 210, 280);
	but_pause_->SetCallbackID(Input::PAUSE);

	(new CosmoButton(this, I18n::t("menu.back"), 210, 410))->SetCallbackID(9000);
}


void JoystickMenu::Poke()
{
	but_weapon1_->SetText(GetButtonLabel(Input::USE_WEAPON_1));

	but_weapon2_->SetText(GetButtonLabel(Input::USE_WEAPON_2));

	but_missile_->SetText(GetButtonLabel(Input::USE_MISSILE));

	but_cooler_->SetText(GetButtonLabel(Input::USE_COOLER));

	but_pause_->SetText(GetButtonLabel(Input::PAUSE));
}


std::wstring JoystickMenu::GetButtonLabel(Input::Action action) const
{
	// FIXME: erreur mémoire sous valgrind
	return str_sprintf(I18n::t("menu.joystick.button_bind").c_str(),
		Input::ActionToString(action), Input::GetInstance().GetJoystickBind(action));
}


void JoystickMenu::EventCallback(int id)
{
	if (id == 9000)
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
