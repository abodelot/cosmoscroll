#include "KeyboardMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


KeyboardMenu::KeyboardMenu()
{
	SetTitle(I18n::t("menu.keyboard.title"));

	but_up_ =      new gui::Button(this, GetKeyLabel(Input::MOVE_UP), 150, 100);
	but_up_->SetCallbackID(Input::MOVE_UP);
	but_down_ =    new gui::Button(this, GetKeyLabel(Input::MOVE_DOWN), 150, 130);
	but_down_->SetCallbackID(Input::MOVE_DOWN);
	but_left_ =    new gui::Button(this, GetKeyLabel(Input::MOVE_LEFT), 150, 160);
	but_left_->SetCallbackID(Input::MOVE_LEFT);
	but_right_ =   new gui::Button(this, GetKeyLabel(Input::MOVE_RIGHT), 150, 190);
	but_right_->SetCallbackID(Input::MOVE_RIGHT);
	but_weapon1_ = new gui::Button(this, GetKeyLabel(Input::USE_WEAPON_1), 150, 220);
	but_weapon1_->SetCallbackID(Input::USE_WEAPON_1);
	but_weapon2_ = new gui::Button(this, GetKeyLabel(Input::USE_WEAPON_2), 150, 250);
	but_weapon2_->SetCallbackID(Input::USE_WEAPON_2);
	but_cooler_ =  new gui::Button(this, GetKeyLabel(Input::USE_COOLER), 150, 280);
	but_cooler_->SetCallbackID(Input::USE_COOLER);
	but_missile_ = new gui::Button(this, GetKeyLabel(Input::USE_MISSILE), 150, 310);
	but_missile_->SetCallbackID(Input::USE_MISSILE);
	but_pause_ =   new gui::Button(this, GetKeyLabel(Input::PAUSE), 150, 340);
	but_pause_->SetCallbackID(Input::PAUSE);

	(new CosmoButton(this, I18n::t("menu.back"), 210, 410))->SetCallbackID(9000);
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


std::wstring KeyboardMenu::GetKeyLabel(Input::Action action) const
{
	Input& input = Input::GetInstance();

	std::wstring label = Input::ActionToString(action);
	label += L" : ";
	// convert key string to wstring
	const std::string str = Input::KeyToString(input.GetKeyboardBind(action));
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	label += wstr;
	return label;
}

