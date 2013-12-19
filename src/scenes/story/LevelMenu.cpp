#include "LevelMenu.hpp"
#include "core/Game.hpp"
#include "core/UserSettings.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "utils/I18n.hpp"


LevelMenu::LevelMenu():
	levels_(LevelManager::getInstance())
{
	SetTitle(_t("menu.story.title"));
	m_credits = new CreditCounterWidget(this);

	gui::FormLayout form(90, 120);
	form.SetSpacing(10, 16);

	opt_levels_ = new gui::OptionList(this);
	form.AddRow(_t("menu.story.select"), opt_levels_);

	lab_progresion_ = new gui::Label(this, "");
	form.AddRow(_t("menu.story.progression"), lab_progresion_);

	gui::VBoxLayout layout(210, 240);
	layout.Add(new CosmoButton(this, _t("menu.story.play")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("menu.story.armory")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("menu.back_main_menu")))->SetCallbackID(0);
}


void LevelMenu::OnFocus()
{
	m_credits->setCredits(UserSettings::getCredits());
	size_t nb_levels = levels_.getLevelCount();
	size_t current = levels_.getCurrent();
	size_t last_unlocked = levels_.getLastUnlocked();

	std::ostringstream progression;
	progression << last_unlocked << "/" << nb_levels;
	lab_progresion_->setString(progression.str());

	// option widget
	opt_levels_->Clear();
	for (size_t i = 1; i <= nb_levels; ++i)
	{
		bool activable = i <= last_unlocked;
		if (activable)
		{
			opt_levels_->AddOption(std::to_string(i));
		}
	}
	opt_levels_->Select(current - 1);
}


void LevelMenu::EventCallback(int id)
{
	switch (id)
	{
		case 0:
			Game::getInstance().setNextScene(Game::SC_MainMenu);
			break;
		case 1: {
			int selected_level = opt_levels_->GetSelectedOptionIndex() + 1;
			levels_.setCurrent(selected_level);
			levels_.initCurrentLevel();

			std::wstring s = I18n::templatize("panel.level", "{level}", selected_level);
			ControlPanel::getInstance().SetGameInfo(s);
			Game::getInstance().setNextScene(Game::SC_IntroLevelScene);
			}
			break;
		case 2:
			Game::getInstance().setNextScene(Game::SC_ArmoryMenu);
			break;
	}
}
