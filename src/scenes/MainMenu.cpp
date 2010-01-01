#include "MainMenu.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/I18n.hpp"
#include "../entities/EntityManager.hpp"


MainMenu::MainMenu()
{
	title_.SetImage(GET_IMG("gui/cosmoscroll-logo"));
	title_.SetPosition((Game::WIDTH - title_.GetSize().x) / 2, 12);

	SetOffsetY(130);

	AddOption(I18n::t("menu.main.story"),   1);
	AddOption(I18n::t("menu.main.arcade"),  2);
	AddOption(I18n::t("menu.main.options"), 3);
	AddOption(I18n::t("menu.main.about"),   4);
	AddOption(I18n::t("menu.main.quit"),    5);

	// init background music at first launch of main menu
	SoundSystem& sound = SoundSystem::GetInstance();
	sound.PlayMusic(sound.GetMusicName());
}


void MainMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.Draw(title_);
}


void MainMenu::Callback(int id)
{
	Game& game = Game::GetInstance();
	switch (id)
	{
		case 1:
			game.SetNextScene(Game::SC_LevelMenu);
			break;
		case 2:
			game.SetNextScene(Game::SC_InGameScene);
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
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
