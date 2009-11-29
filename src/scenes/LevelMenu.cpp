#include "LevelMenu.hpp"
#include "../core/Game.hpp"
#include "../core/LevelManager.hpp"
#include "../core/ControlPanel.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/StringUtils.hpp"


LevelMenu::LevelMenu():
	levels_(LevelManager::GetInstance())
{
	SetOffset(Game::MARGIN_X, 100);
}


void LevelMenu::Poke()
{
	BaseMenu::Clear();
	int last = levels_.CountLevel();
	int current = levels_.GetCurrent();
	int last_unlocked = levels_.GetLastUnlocked();
	for (int i = 1; i <= last; ++i)
	{
		bool activable = i <= last_unlocked;
		AddOption(str_sprintf("Niveau %d", i), i, activable);
	}
	AddOption("Retour au menu principal", 0);
	SelectItem(current - 1);
}


void LevelMenu::Callback(int id)
{
	if (id == 0)
	{
		Game::GetInstance().SetNextScene(Game::SC_MainMenu);
	}
	else
	{
		// id refers to the current level index
		EntityManager& entities = EntityManager::GetInstance();
		entities.SetBackgroundColor(
			levels_.GetTopColor(id),
			levels_.GetBottomColor(id)
		);
		entities.SetMode(EntityManager::MODE_STORY);
		entities.RespawnPlayer();
		levels_.SetCurrent(id);
		levels_.LoadCurrent();

		ControlPanel::GetInstance().SetGameInfo(str_sprintf("Niveau %d", id));
		Game::GetInstance().SetNextScene(Game::SC_IntroLevelScene);
	}
}
