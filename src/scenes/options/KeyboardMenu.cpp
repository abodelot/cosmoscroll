#include "KeyboardMenu.hpp"
#include "scenes/ConfigButton.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


KeyboardMenu::KeyboardMenu()
{
	SetTitle(_t("menu.keyboard.title"));

	AddRow(Input::MOVE_UP,      &but_up_, 100);
	AddRow(Input::MOVE_DOWN,    &but_down_, 130);
	AddRow(Input::MOVE_LEFT,    &but_left_, 160);
	AddRow(Input::MOVE_RIGHT,   &but_right_, 190);
	AddRow(Input::USE_WEAPON_1, &but_weapon1_, 220);
	AddRow(Input::USE_WEAPON_2, &but_weapon2_, 250);
	AddRow(Input::USE_COOLER,   &but_cooler_, 280);
	AddRow(Input::USE_MISSILE,  &but_missile_, 310);
	AddRow(Input::PAUSE,        &but_pause_, 340);

	(new CosmoButton(this, _t("menu.back"), 210, 410))->SetCallbackID(9000);
}


void KeyboardMenu::OnFocus()
{
	BaseMenu::OnFocus();
	but_up_->SetText(GetKeyLabel(Input::MOVE_UP));
	but_down_->SetText(GetKeyLabel(Input::MOVE_DOWN));
	but_left_->SetText(GetKeyLabel(Input::MOVE_LEFT));
	but_right_->SetText(GetKeyLabel(Input::MOVE_RIGHT));
	but_weapon1_->SetText(GetKeyLabel(Input::USE_WEAPON_1));
	but_weapon2_->SetText(GetKeyLabel(Input::USE_WEAPON_2));
	but_cooler_->SetText(GetKeyLabel(Input::USE_COOLER));
	but_missile_->SetText(GetKeyLabel(Input::USE_MISSILE));
	but_pause_->SetText(GetKeyLabel(Input::PAUSE));
}



void KeyboardMenu::EventCallback(int id)
{
	if (id == 9000)
	{
		Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
	}
	else
	{
		// Input::Action enumerations are used as menu ids
		Input& input = Input::GetInstance();
		input.AskUserInput(Input::KEYBOARD, (Input::Action) id);
		OnFocus();
	}
}


void KeyboardMenu::AddRow(Input::Action action, gui::Button** button, int y)
{
	new gui::Label(this, Input::ActionToString(action), 150, y);
	*button = new ConfigButton(this, GetKeyLabel(action), 350, y);
	(*button)->SetCallbackID(action);
}


const char* KeyboardMenu::GetKeyLabel(Input::Action action) const
{
	return Input::KeyToString(Input::GetInstance().GetKeyboardBind(action));
}

