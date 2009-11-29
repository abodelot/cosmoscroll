#include "OptionMenu.hpp"
#include "../core/Game.hpp"


OptionMenu::OptionMenu()
{
	SetOffset(Game::MARGIN_X, 100);

	AddOption("Configuration clavier",  1);
	AddOption("Configuration joystick", 2);
	AddOption("Sons & musique",         3);
	AddOption(L"Préférences",           4);
	AddOption("Retour",                 5);
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

