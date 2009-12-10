#include "PauseMenu.hpp"
#include "../core/Game.hpp"
#include "../core/ControlPanel.hpp"
#include "../entities/EntityManager.hpp"


PauseMenu::PauseMenu()
{
	SetTitle(L"Pause", 130);
	SetOffset(200, 210);

	AddOption("Reprendre la partie",       1);
	AddOption("Options",                   2);
	AddOption("Revenir au menu principal", 3);
	AddOption("Quitter le jeu",            4);
}


void PauseMenu::Callback(int id)
{
	Game& game = Game::GetInstance();
	switch (id)
	{
		case 1:
			game.SetNextScene(Game::SC_InGameScene);
			break;
		case 2:
			game.SetNextScene(Game::SC_OptionMenu);
			break;
		case 3:
			game.SetNextScene(Game::SC_MainMenu);
			break;
		case 4:
			game.Quit();
			break;
	}
}


void PauseMenu::Update(float frametime)
{
	ParticleSystem::GetInstance().Update(frametime);
}


void PauseMenu::Show(sf::RenderTarget& target) const
{
	target.Draw(ControlPanel::GetInstance());
	target.Draw(EntityManager::GetInstance());
	BaseMenu::DrawItems(target);
}

