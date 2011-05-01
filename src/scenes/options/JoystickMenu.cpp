#include "JoystickMenu.hpp"
#include "core/Game.hpp"
#include "core/Input.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"


JoystickMenu::JoystickMenu()
{
	SetTitle(I18n::t("menu.joystick.title"));

	but_weapon1_ = new gui::Button(this, GetButtonLabel(Input::USE_WEAPON_1), 125, 120);
	but_weapon1_->SetCallbackID(Input::USE_WEAPON_1);
	but_weapon2_ = new gui::Button(this, GetButtonLabel(Input::USE_WEAPON_2), 125, 160);
	but_weapon2_->SetCallbackID(Input::USE_WEAPON_2);
	but_missile_ = new gui::Button(this, GetButtonLabel(Input::USE_MISSILE), 125, 200);
	but_missile_->SetCallbackID(Input::USE_MISSILE);
	but_cooler_ =  new gui::Button(this, GetButtonLabel(Input::USE_COOLER), 125, 240);
	but_cooler_->SetCallbackID(Input::USE_COOLER);
	but_pause_ =   new gui::Button(this, GetButtonLabel(Input::PAUSE), 125, 280);
	but_pause_->SetCallbackID(Input::PAUSE);

	new gui::Label(this, I18n::t("menu.joystick.sensitivity"), 125, 340);
	sl_joystick_ = new gui::Slider(this, 340, 344, 160);
	sl_joystick_->SetCallbackID(9000);

	(new CosmoButton(this, I18n::t("menu.back"), 210, 410))->SetCallbackID(9001);
}


void JoystickMenu::OnFocus()
{
	BaseMenu::OnFocus();
	but_weapon1_->SetText(GetButtonLabel(Input::USE_WEAPON_1));

	but_weapon2_->SetText(GetButtonLabel(Input::USE_WEAPON_2));

	but_missile_->SetText(GetButtonLabel(Input::USE_MISSILE));

	but_cooler_->SetText(GetButtonLabel(Input::USE_COOLER));

	but_pause_->SetText(GetButtonLabel(Input::PAUSE));

	sl_joystick_->SetValue(100 - Input::GetInstance().GetSensitivity());
}


std::wstring JoystickMenu::GetButtonLabel(Input::Action action) const
{
	// FIXME: erreur mémoire sous valgrind
	return str_sprintf(I18n::t("menu.joystick.button_bind").c_str(),
		Input::ActionToString(action), Input::GetInstance().GetJoystickBind(action));
}


void JoystickMenu::EventCallback(int id)
{
	Input& input = Input::GetInstance();
	switch (id)
	{
		case 9000:
			input.SetSensitivity(100 - sl_joystick_->GetValue());
			break;
		case 9001:
			Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
			break;
		default:
			// Input::Action enumerations are used as menu ids
			input.AskUserInput(Input::JOYSTICK, (Input::Action) id);
			OnFocus();
	}
}
