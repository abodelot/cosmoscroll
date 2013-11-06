#include "ArcadeMenu.hpp"
#include "utils/I18n.hpp"
#include "core/Game.hpp"
#include "entities/EntityManager.hpp"


ArcadeMenu::ArcadeMenu()
{
	SetTitle(_t("menu.arcade.title"));

	gui::VBoxLayout layout(210, 120);
	layout.Add(new CosmoButton(this, _t("menu.arcade.play")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("menu.arcade.best_scores")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("menu.back_main_menu")))->SetCallbackID(3);
}


void ArcadeMenu::EventCallback(int id)
{
	Game& game = Game::getInstance();
	switch (id)
	{
		case 1:
			game.setNextScene(Game::SC_InGameScene);
			EntityManager::getInstance().InitMode(EntityManager::MODE_ARCADE);
			break;
		case 2:
			game.setNextScene(Game::SC_BestScoresMenu);
			break;
		case 3:
			game.setNextScene(Game::SC_MainMenu);
			break;
	}
}
