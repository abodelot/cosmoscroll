#include "MainMenu.hpp"
#include "../core/Game.hpp"
#include "../core/SoundSystem.hpp"


MainMenu::MainMenu()
{
	title_.SetImage(GET_IMG("cosmoscroll-logo"));
	title_.SetPosition((Game::WIDTH - title_.GetSize().x) / 2, 12);

	SetOffset(Game::MARGIN_X, 130);

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
			EntityManager::GetInstance().SetMode(EntityManager::MODE_ARCADE);
			EntityManager::GetInstance().RespawnPlayer();
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


	/*
		case 2:
			mode_ = ARCADE;
			next = PLAY;
			panel_.SetGameInfo(str_sprintf("Record : %02d:%02d",
				(int) best_arcade_time_ / 60,
				(int) best_arcade_time_ % 60).c_str());
			Init();
			timer_ = 0.f;
			p_StopPlay_ = &Game::ArcadeMoreBadGuys;

	*/
