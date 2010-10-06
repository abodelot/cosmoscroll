#include "SettingsMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/I18n.hpp"


SettingsMenu::SettingsMenu()
{
	new gui::Label(this, L"Plein écran :", 100, 200);
	cb_fullscreen_ = new gui::CheckBox(this, 300, 200);
	cb_fullscreen_->SetCallbackID(1);

	(new CosmoButton(this, I18n::t("menu.back"), 210, 340))->SetCallbackID(0);
}


void SettingsMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
		case  1:
			//Game::GetInstance().SetFullscreen(cb_fullscreen_->Checked());
			break;
	}
}
