#include "OptionMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


OptionMenu::OptionMenu()
{
	SetTitle(_t("menu.options.title"));

	gui::VBoxLayout layout(210, 120);

	layout.Add(new CosmoButton(this, _t("menu.options.keyboard")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("menu.options.joystick")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("menu.options.audio")))->SetCallbackID(3);
	layout.Add(new CosmoButton(this, _t("menu.options.settings")))->SetCallbackID(4);
	layout.Add(new CosmoButton(this, _t("menu.back")))->SetCallbackID(5);
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

