#include "LevelMenu.hpp"
#include "core/Game.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "entities/EntityManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"


LevelMenu::LevelMenu():
	levels_(LevelManager::GetInstance())
{
	SetTitle(I18n::t("menu.story.title"));

	new gui::Label(this, I18n::t("menu.story.choice"), 100, 170);
	opt_levels_ = new gui::OptionList(this, 380, 170);

	lab_progresion_ = new gui::Label(this, "Progression", 100, 200);

	CosmoButton* but = NULL;
	but = new CosmoButton(this, I18n::t("menu.story.play"), 210, 240);
	but->SetCallbackID(1);
	but = new CosmoButton(this, I18n::t("menu.back_main_menu"), 210, 300);
	but->SetCallbackID(0);
}


void LevelMenu::OnFocus()
{
	BaseMenu::OnFocus();
	int last = levels_.CountLevel();
	int current = levels_.GetCurrent();
	int last_unlocked = levels_.GetLastUnlocked();

	lab_progresion_->SetText(str_sprintf("Progression : %d / %d", last_unlocked, last));
	opt_levels_->Clear();
	for (int i = 1; i <= last; ++i)
	{
		bool activable = i <= last_unlocked;
		if (activable)
		{
			opt_levels_->AddOption(str_sprintf(I18n::t("menu.story.level").c_str(), i));
		}
	}
	opt_levels_->Select(current - 1);
}


void LevelMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
		case 1: {
			int level_select = opt_levels_->GetSelectedOptionIndex() + 1;
			EntityManager& entities = EntityManager::GetInstance();
			entities.SetBackgroundColor(
				levels_.GetTopColor(level_select),
				levels_.GetBottomColor(level_select)
			);
			levels_.SetCurrent(level_select);
			levels_.LoadCurrent();
			entities.InitMode(EntityManager::MODE_STORY);

			ControlPanel::GetInstance().SetGameInfo(str_sprintf(I18n::t("panel.level").c_str(), level_select));
			Game::GetInstance().SetNextScene(Game::SC_IntroLevelScene);
			}
			break;
	}
}
