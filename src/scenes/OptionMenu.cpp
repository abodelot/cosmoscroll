#include "OptionMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/I18n.hpp"


OptionMenu::OptionMenu()
{
	SetTitle(I18n::t("menu.options.title"));
	SetOffsetY(100);

	AddOption(I18n::t("menu.options.keyboard"), 1);
	AddOption(I18n::t("menu.options.joystick"), 2);
	AddOption(I18n::t("menu.options.audio"),    3);
	//AddOption(L"Préférences",           4);
	AddOption(I18n::t("menu.back"),             5);
}


void OptionMenu::Callback(int id)
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
			//game.SetNextScene(Game::SC_SettingsMenu);
			break;
		case 5:
			game.SetNextScene(Game::SC_MainMenu);
			break;
	}
}

