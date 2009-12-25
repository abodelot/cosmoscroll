#include "MainMenu.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../entities/EntityManager.hpp"


MainMenu::MainMenu()
{
	title_.SetImage(GET_IMG("gui/cosmoscroll-logo"));
	title_.SetPosition((Game::WIDTH - title_.GetSize().x) / 2, 12);

	SetOffsetY(130);

	AddOption("Mode Histoire", 1);
	AddOption("Mode Arcade",   2);
	AddOption("Options",       3);
	AddOption(L"À Propos",     4);
	AddOption("Quitter",       5);

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
