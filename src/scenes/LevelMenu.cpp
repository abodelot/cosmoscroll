#include "LevelMenu.hpp"
#include "../core/Game.hpp"
#include "../core/LevelManager.hpp"
#include "../core/ControlPanel.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"


LevelMenu::LevelMenu():
	levels_(LevelManager::GetInstance())
{
	SetTitle(I18n::t("menu.level.title"));
	SetOffsetY(100);
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
		AddOption(str_sprintf(I18n::t("menu.level.level").c_str(), i), i, activable);
	}
	AddOption(I18n::t("menu.back"), 0);
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
		levels_.SetCurrent(id);
		levels_.LoadCurrent();
		entities.InitMode(EntityManager::MODE_STORY);

		ControlPanel::GetInstance().SetGameInfo(str_sprintf(I18n::t("panel.level").c_str(), id));
		Game::GetInstance().SetNextScene(Game::SC_IntroLevelScene);
	}
}
