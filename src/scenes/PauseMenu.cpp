#include "PauseMenu.hpp"
#include "../core/Game.hpp"
#include "../core/ControlPanel.hpp"
#include "../core/ParticleSystem.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/I18n.hpp"


PauseMenu::PauseMenu()
{
	SetTitle(_t("menu.pause.title"), 120);
	SetBackground(sf::Sprite());

	(new CosmoButton(this, _t("menu.pause.resume"), 210, 200))->SetCallbackID(1);
	(new CosmoButton(this, _t("menu.back_main_menu"), 210, 250))->SetCallbackID(2);
	(new CosmoButton(this, _t("menu.pause.quit"), 210, 300))->SetCallbackID(3);
}


void PauseMenu::EventCallback(int id)
{
	Game& game = Game::GetInstance();
	switch (id)
	{
		case 1:
			game.SetNextScene(Game::SC_InGameScene);
			break;
		case 2:
			game.SetNextScene(Game::SC_MainMenu);
			break;
		case 3:
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
	target.Draw(GetTitle());
	gui::Menu::Show(target);
}

