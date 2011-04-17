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

	new gui::Label(this, I18n::t("menu.story.select"), 90, 120);
	opt_levels_ = new gui::OptionList(this, 315, 120);

	lab_progresion_ = new gui::Label(this, "", 90, 150);

	lab_hardcore_ = new gui::Label(this, "Mode hardcore", 90, 180);
	lab_hardcore_->SetVisible(false);
	cbx_hardcore_ = new gui::CheckBox(this, 315, 180);
	cbx_hardcore_->SetVisible(false);

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
	printf("current is %d\n", current);
	int last_unlocked = levels_.GetLastUnlocked();

	if (levels_.AllLevelsCompleted())
	{
		if (!lab_hardcore_->IsVisible())
		{
			lab_hardcore_->SetVisible(true);
			cbx_hardcore_->SetVisible(true);
			cbx_hardcore_->SetCallbackID(2);
		}
		cbx_hardcore_->Check(levels_.IsHardcoreEnabled());
	}

	lab_progresion_->SetText(str_sprintf(I18n::t("menu.story.progression").c_str(), last_unlocked, last));
	opt_levels_->Clear();
	for (int i = 1; i <= last; ++i)
	{
		bool activable = i <= last_unlocked;
		if (activable)
		{
			opt_levels_->AddOption(str_sprintf(" %d ", i));
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
			levels_.SetCurrent(level_select);
			levels_.LoadCurrent();
			EntityManager& entities = EntityManager::GetInstance();
			entities.SetBackgroundColor(
				levels_.GetTopColor(),
				levels_.GetBottomColor()
			);

			entities.InitMode(EntityManager::MODE_STORY);

			ControlPanel::GetInstance().SetGameInfo(str_sprintf(I18n::t("panel.level").c_str(), level_select));
			Game::GetInstance().SetNextScene(Game::SC_IntroLevelScene);
			}
			break;
		case 2:
			levels_.EnableHardcore(cbx_hardcore_->Checked());
			// rebuild level list widget
			OnFocus();
			break;

	}
}
