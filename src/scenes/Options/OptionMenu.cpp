#include "OptionMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


OptionMenu::OptionMenu()
{
	setTitle(_t("options.title"));

	gui::VBoxLayout layout(210, 120);

	layout.Add(new CosmoButton(this, _t("options.keyboard")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("options.joystick")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("options.audio")))->SetCallbackID(3);
	layout.Add(new CosmoButton(this, _t("options.settings")))->SetCallbackID(4);
	layout.Add(new CosmoButton(this, _t("back")))->SetCallbackID(5);
}


void OptionMenu::EventCallback(int id)
{
	Game& game = Game::getInstance();
	switch (id)
	{
		case 1:
			game.setCurrentScreen(Game::SC_KeyboardMenu);
			break;
		case 2:
			game.setCurrentScreen(Game::SC_JoystickMenu);
			break;
		case 3:
			game.setCurrentScreen(Game::SC_AudioMenu);
			break;
		case 4:
			game.setCurrentScreen(Game::SC_SettingsMenu);
			break;
		case 5:
			game.setCurrentScreen(Game::SC_MainMenu);
			break;
	}
}

