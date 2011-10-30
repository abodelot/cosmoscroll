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
	SetTitle(_t("menu.story.title"));

	gui::FormLayout form(90, 120);
	form.SetSpacing(10, 16);

	opt_levels_ = new gui::OptionList(this);
	form.AddRow(_t("menu.story.select"), opt_levels_);

	lab_progresion_ = new gui::Label(this, "");
	form.AddRow(_t("menu.story.progression"), lab_progresion_);

	cbx_hardcore_ = new gui::CheckBox(this);
	cbx_hardcore_->SetVisible(false);
	form.AddRow("Mode hardcore", cbx_hardcore_);
	lab_hardcore_ = form.GetLabelAt(2);
	lab_hardcore_->SetVisible(false);

	gui::VBoxLayout layout(210, 240);
	layout.Add(new CosmoButton(this, _t("menu.story.play")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("menu.story.armory")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("menu.back_main_menu")))->SetCallbackID(0);
}


void LevelMenu::OnFocus()
{
	CreditCounterBase::OnFocus();
	int last = levels_.CountLevel();

	levels_.VerifyCurrent();	// FIX: On charge dorénavant la config avant le levelmanager.

	int current = levels_.GetCurrent();
	if (levels_.IsHardcoreEnabled())
		current -= last;

	int last_unlocked = levels_.GetLastUnlocked();
	if (levels_.AllLevelsCompleted())
	{
		if (levels_.IsHardcoreEnabled())
			last_unlocked -= last;

		// enable/disable hardcore
		if (!cbx_hardcore_->IsVisible())
		{
			lab_hardcore_->SetVisible(true);
			cbx_hardcore_->SetVisible(true);
			cbx_hardcore_->SetCallbackID(3);
		}
		cbx_hardcore_->Check(levels_.IsHardcoreEnabled());
	}

	if (last_unlocked > levels_.CountLevel())
	{
		last_unlocked = levels_.CountLevel();
	}

	std::ostringstream progression;
	progression << last_unlocked << "/" << last;
	lab_progresion_->SetText(progression.str());

	// option widget
	opt_levels_->Clear();
	for (int i = 1; i <= last; ++i)
	{
		bool activable = i <= last_unlocked;
		if (activable)
		{
			opt_levels_->AddOption(to_string(i, 4, ' ') + "   ");
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
			int selected_level = opt_levels_->GetSelectedOptionIndex() + 1;
			if (cbx_hardcore_->Checked())
				levels_.SetCurrent(selected_level + levels_.CountLevel());
			else
				levels_.SetCurrent(selected_level);

			levels_.LoadCurrent();
			EntityManager::GetInstance().InitMode(EntityManager::MODE_STORY);

			std::wstring s = wstr_replace(_t("panel.level"), L"{level}", to_wstring(selected_level));
			ControlPanel::GetInstance().SetGameInfo(s);
			Game::GetInstance().SetNextScene(Game::SC_IntroLevelScene);
			}
			break;
		case 2:
			Game::GetInstance().SetNextScene(Game::SC_ArmoryMenu);
			break;
		case 3:
			levels_.EnableHardcore(cbx_hardcore_->Checked());
			// rebuild level list widget
			OnFocus();
			break;

	}
}
