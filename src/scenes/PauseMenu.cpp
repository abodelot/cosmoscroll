#include "PauseMenu.hpp"
#include "../core/Game.hpp"
#include "../core/ControlPanel.hpp"
#include "../entities/EntityManager.hpp"


PauseMenu::PauseMenu()
{
	SetOffset(200, 210);

	AddOption("Reprendre la partie",       1);
	AddOption("Options",                   2);
	AddOption("Revenir au menu principal", 3);
	AddOption("Quitter le jeu",            4);

	title_.SetText("Pause");
	title_.SetPosition(200, 130);
	title_.SetSize(60);
	title_.SetFont(GET_FONT());
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
	target.Draw(title_);
}

