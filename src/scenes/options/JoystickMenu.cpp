#include <sstream>

#include "JoystickMenu.hpp"
#include "core/Game.hpp"
#include "core/Input.hpp"
#include "scenes/ConfigButton.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"


JoystickMenu::JoystickMenu()
{
	SetTitle(_t("menu.joystick.title"));

	gui::FormLayout form(110, 110);
	form.SetSpacing(20, 16);
	AddRow(form, Input::USE_WEAPON_1, &but_weapon1_);
	AddRow(form, Input::USE_WEAPON_2, &but_weapon2_);
	AddRow(form, Input::USE_MISSILE, &but_missile_);
	AddRow(form, Input::USE_COOLER, &but_cooler_);
	AddRow(form, Input::PAUSE, &but_pause_);

	sl_joystick_ = new gui::Slider(this, 160);
	sl_joystick_->SetCallbackID(9000);
	form.AddRow(_t("menu.joystick.sensitivity"), sl_joystick_);

	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->SetPosition(210, 410);
	b->SetCallbackID(9001);
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

void JoystickMenu::AddRow(gui::FormLayout& form, Input::Action action, gui::Button** button)
{
	*button = new ConfigButton(this, GetButtonLabel(action));
	(*button)->SetCallbackID(action);

	form.AddRow(Input::ActionToString(action), *button);
}


std::wstring JoystickMenu::GetButtonLabel(Input::Action action) const
{
	std::wostringstream oss;
	oss << _t("menu.joystick.button") << L" " << Input::GetInstance().GetJoystickBind(action);
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
