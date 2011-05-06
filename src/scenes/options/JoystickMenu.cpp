#include <sstream>

#include "JoystickMenu.hpp"
#include "core/Game.hpp"
#include "core/Input.hpp"
#include "scenes/ConfigButton.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"

#define COL_1 125
#define COL_2 340

JoystickMenu::JoystickMenu()
{
	SetTitle(I18n::t("menu.joystick.title"));

	AddRow(Input::USE_WEAPON_1, &but_weapon1_, 120);
	AddRow(Input::USE_WEAPON_2, &but_weapon2_, 160);
	AddRow(Input::USE_MISSILE, &but_missile_, 200);
	AddRow(Input::USE_COOLER, &but_cooler_, 240);
	AddRow(Input::PAUSE, &but_pause_, 280);

	new gui::Label(this, I18n::t("menu.joystick.sensitivity"), COL_1, 340);
	sl_joystick_ = new gui::Slider(this, COL_2, 344, 160);
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

void JoystickMenu::AddRow(Input::Action action, gui::Button** button, int y)
{
	new gui::Label(this, Input::ActionToString(action), COL_1, y);
	*button = new ConfigButton(this, GetButtonLabel(action), COL_2, y);
	(*button)->SetCallbackID(action);
}


std::wstring JoystickMenu::GetButtonLabel(Input::Action action) const
{
	std::wostringstream oss;
	oss << I18n::t("menu.joystick.button") << " " << Input::GetInstance().GetJoystickBind(action);
	return oss.str();
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
