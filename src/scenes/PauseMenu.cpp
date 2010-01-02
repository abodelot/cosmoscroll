#include "PauseMenu.hpp"
#include "../core/Game.hpp"
#include "../core/ControlPanel.hpp"
#include "../core/ParticleSystem.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/I18n.hpp"


PauseMenu::PauseMenu()
{
	SetTitle(I18n::t("menu.pause.title"), 130);
	SetOffsetY(210);

	AddOption(I18n::t("menu.pause.resume"),   1);
	AddOption(I18n::t("menu.back_main_menu"), 2);
	AddOption(I18n::t("menu.pause.quit"),     3);
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
	BaseMenu::DrawItems(target);
}

