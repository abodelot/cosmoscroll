#include "SettingsMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


SettingsMenu::SettingsMenu()
{
	SetTitle(_t("menu.settings.title"));
	new gui::Label(this, _t("menu.settings.fullscreen"), 100, 180);
	cb_fullscreen_ = new gui::CheckBox(this, 300, 180);
	cb_fullscreen_->SetCallbackID(1);
	cb_fullscreen_->Check(Game::GetInstance().IsFullscreen());

	new gui::Label(this, _t("menu.settings.language"), 100, 250);

	opt_languages_ = new gui::OptionList(this, 300, 250);
	opt_languages_->AddOption(L"English", "en");
	opt_languages_->AddOption(L"Français", "fr");
	opt_languages_->SelectByValue(I18n::GetInstance().GetCurrentCode());
	opt_languages_->SetCallbackID(2);
	new gui::Label(this, "(Game must be restarted)", 300, 280);

	(new CosmoButton(this, _t("menu.back"), 210, 340))->SetCallbackID(0);
}


void SettingsMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			Game::GetInstance().SetNextScene(Game::SC_OptionMenu);
			break;
		case  1:
			Game::GetInstance().SetFullscreen(cb_fullscreen_->Checked());
			break;
		case 2:
			I18n::GetInstance().LoadFromCode(opt_languages_->GetSelectedOption());
			break;
	}
}
