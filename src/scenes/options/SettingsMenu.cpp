#include "SettingsMenu.hpp"
#include "core/Game.hpp"
#include "core/ControlPanel.hpp"
#include "utils/I18n.hpp"


SettingsMenu::SettingsMenu()
{
	SetTitle(_t("menu.settings.title"));

	form_.SetOffset(90, 140);
	form_.SetSpacing(10, 25);
	form_.SetLabelAlignment(gui::Align::RIGHT);

	cb_fullscreen_ = new gui::CheckBox(this);
	cb_fullscreen_->SetCallbackID(1);
	cb_fullscreen_->Check(Game::GetInstance().IsFullscreen());
	form_.AddRow(_t("menu.settings.fullscreen"), cb_fullscreen_);

	cb_vsync_ = new gui::CheckBox(this);
	cb_vsync_->SetCallbackID(2);
	cb_vsync_->Check(Game::GetInstance().IsVerticalSync());
	form_.AddRow(_t("menu.settings.vsync"), cb_vsync_);

	opt_languages_ = new gui::OptionList(this);
	opt_languages_->AddOption(L"English", "en");
	opt_languages_->AddOption(L"Français", "fr");
	opt_languages_->AddOption(L"Deutsch", "de");
	opt_languages_->AddOption(L"Pусский", "ru");
	opt_languages_->SelectByValue(I18n::GetInstance().GetCurrentCode());
	opt_languages_->SetCallbackID(3);
	form_.AddRow(_t("menu.settings.language"), opt_languages_);

	but_back_ = new CosmoButton(this, _t("menu.back"));
	but_back_->setPosition(210, 340);
	but_back_->SetCallbackID(0);
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
			Game::GetInstance().SetVerticalSync(cb_vsync_->Checked());
			break;
		case 3:
			I18n::GetInstance().LoadFromCode(opt_languages_->GetSelectedOption());
			// delete other scenes
			Game::GetInstance().ReloadScenes();
			ControlPanel::GetInstance().RefreshTextTranslations();
			// re-load i18ned texts
			SetTitle(_t("menu.settings.title"));
			form_.GetLabelAt(0)->setString(_t("menu.settings.fullscreen"));
			form_.GetLabelAt(1)->setString(_t("menu.settings.vsync"));
			form_.GetLabelAt(2)->setString(_t("menu.settings.language"));
			form_.AlignRows();

			but_back_->setString(_t("menu.back"));
			break;
	}
}
