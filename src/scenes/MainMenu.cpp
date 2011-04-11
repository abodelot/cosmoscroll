#include "MainMenu.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/I18n.hpp"


MainMenu::MainMenu()
{
	title_.SetImage(GET_IMG("gui/cosmoscroll-logo"));
	title_.SetPosition((Game::WIDTH - title_.GetSize().x) / 2, 12);

	(new CosmoButton(this, I18n::t("menu.main.story"),   210, 120))->SetCallbackID(1);
	(new CosmoButton(this, I18n::t("menu.main.arcade"),  210, 170))->SetCallbackID(2);
	(new CosmoButton(this, I18n::t("menu.main.options"), 210, 220))->SetCallbackID(3);
	(new CosmoButton(this, I18n::t("menu.main.about"),   210, 270))->SetCallbackID(4);
	(new CosmoButton(this, I18n::t("menu.main.quit"),    210, 320))->SetCallbackID(5);

	// init background music at first launch of main menu
	SoundSystem& sound = SoundSystem::GetInstance();
	sound.PlayMusic(sound.GetMusicName());
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
