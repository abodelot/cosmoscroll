#include "OptionMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/I18n.hpp"


OptionMenu::OptionMenu()
{
	SetTitle(I18n::t("menu.options.title"));

	(new CosmoButton(this, I18n::t("menu.options.keyboard"), 210, 120))->SetCallbackID(1);
	(new CosmoButton(this, I18n::t("menu.options.joystick"), 210, 170))->SetCallbackID(2);
	(new CosmoButton(this, I18n::t("menu.options.audio"),    210, 220))->SetCallbackID(3);
	(new CosmoButton(this, I18n::t("menu.options.settings"),  210, 270))->SetCallbackID(4);
	(new CosmoButton(this, I18n::t("menu.back"), 210, 320))->SetCallbackID(5);
}


void OptionMenu::EventCallback(int id)
{
	Game& game = Game::GetInstance();
	switch (id)
	{
		case 1:
			game.SetNextScene(Game::SC_KeyboardMenu);
			break;
		case 2:
			game.SetNextScene(Game::SC_JoystickMenu);
			break;
		case 3:
			game.SetNextScene(Game::SC_AudioMenu);
			break;
		case 4:
			game.SetNextScene(Game::SC_SettingsMenu);
			break;
		case 5:
			game.SetNextScene(Game::SC_MainMenu);
			break;
	}
}

