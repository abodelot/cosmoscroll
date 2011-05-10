#include "ArcadeMenu.hpp"
#include "utils/I18n.hpp"
#include "core/Game.hpp"
#include "entities/EntityManager.hpp"


ArcadeMenu::ArcadeMenu()
{
	SetTitle(_t("menu.arcade.title"));

	(new CosmoButton(this, _t("menu.arcade.play"),   210, 120))->SetCallbackID(1);
	(new CosmoButton(this, _t("menu.arcade.best_scores"),  210, 170))->SetCallbackID(2);
	(new CosmoButton(this, _t("menu.back_main_menu"), 210, 220))->SetCallbackID(3);
}


void ArcadeMenu::EventCallback(int id)
{
	Game& game = Game::GetInstance();
	switch (id)
	{
		case 1:
			game.SetNextScene(Game::SC_InGameScene);
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
			break;
		case 2:
			game.SetNextScene(Game::SC_BestScoresMenu);
			break;
		case 3:
			game.SetNextScene(Game::SC_MainMenu);
			break;
	}
}
