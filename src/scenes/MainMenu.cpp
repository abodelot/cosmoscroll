#include "MainMenu.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "utils/Resources.hpp"
#include "utils/I18n.hpp"


MainMenu::MainMenu()
{
	title_.SetImage(Resources::GetImage("gui/cosmoscroll-logo.png"));
	title_.SetPosition((Game::WIDTH - title_.GetSize().x) / 2, 12);

	gui::VBoxLayout layout(210, 120);
	layout.SetSpacing(0, 10);

	layout.Add(new CosmoButton(this, _t("menu.main.story")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("menu.main.arcade")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("menu.main.options")))->SetCallbackID(3);
	layout.Add(new CosmoButton(this, _t("menu.main.about")))->SetCallbackID(4);
	layout.Add(new CosmoButton(this, _t("menu.main.quit")))->SetCallbackID(5);
}


void MainMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.Draw(title_);
}


void MainMenu::EventCallback(int id)
{
	Game& game = Game::GetInstance();
	switch (id)
	{
		case 1:
			game.SetNextScene(Game::SC_LevelMenu);
			break;
		case 2:
			game.SetNextScene(Game::SC_ArcadeMenu);
			break;
		case 3:
			game.SetNextScene(Game::SC_OptionMenu);
			break;
		case 4:
			game.SetNextScene(Game::SC_AboutMenu);
			break;
		case 5:
			game.Quit();
			break;
	}
}
