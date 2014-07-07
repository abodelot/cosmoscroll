#include "LevelMenu.hpp"
#include "core/Game.hpp"
#include "core/UserSettings.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "utils/I18n.hpp"


LevelMenu::LevelMenu():
	m_levels(LevelManager::getInstance())
{
	setTitle(_t("levels.title"));
	m_credits = new CreditCounterWidget(this);

	gui::FormLayout form(90, 120);
	form.SetSpacing(10, 16);

	m_opt_levels = new gui::OptionList(this);
	form.AddRow(_t("levels.select"), m_opt_levels);

	m_lab_progresion = new gui::Label(this, "");
	form.AddRow(_t("levels.progression"), m_lab_progresion);

	gui::VBoxLayout layout(210, 240);
	layout.Add(new CosmoButton(this, _t("levels.play")))->SetCallbackID(1);
	layout.Add(new CosmoButton(this, _t("levels.armory")))->SetCallbackID(2);
	layout.Add(new CosmoButton(this, _t("back_main_menu")))->SetCallbackID(0);
}


void LevelMenu::onFocus()
{
	m_credits->setCredits(UserSettings::getCredits());
	size_t nb_levels = m_levels.getLevelCount();
	size_t current = m_levels.getCurrent();
	size_t last_unlocked = m_levels.getLastUnlocked();

	std::ostringstream progression;
	progression << last_unlocked << "/" << nb_levels;
	m_lab_progresion->setString(progression.str());

	// option widget
	m_opt_levels->Clear();
	for (size_t i = 1; i <= nb_levels; ++i)
	{
		bool activable = i <= last_unlocked;
		if (activable)
		{
			m_opt_levels->AddOption(std::to_string(i));
		}
	}
	m_opt_levels->Select(current - 1);
}


void LevelMenu::EventCallback(int id)
{
	switch (id)
	{
		// Back to main menu
		case 0:
			Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
			break;

		// Launch selected level
		case 1:
		{
			int selected_level = m_opt_levels->GetSelectedOptionIndex() + 1;
			// Load selected level in the level manager
			m_levels.setCurrent(selected_level);
			m_levels.initCurrentLevel();

#ifdef DEBUG
	printf("level %u (music: %s)\n", selected_level, m_levels.getMusicName());
	printf(" - available points: %d\n", m_levels.getTotalPoints());
	printf(" - entities:         %d\n", m_levels.getSpawnQueueSize());
	printf(" - duration:      %02d:%02d\n", (int) m_levels.getDuration() / 60, (int) m_levels.getDuration() % 60);
#endif
			// Init control panel
			std::wstring s = I18n::templatize("panel.level", "{level}", selected_level);
			ControlPanel::getInstance().SetGameInfo(s);
			// Init entity manager
			EntityManager::getInstance().setMode(EntityManager::LEVELS_MODE);
			Game::getInstance().setCurrentScreen(Game::SC_PlayScreen);
			break;
		}
		// Armory menu
		case 2:
			Game::getInstance().setCurrentScreen(Game::SC_ArmoryMenu);
			break;
	}
}
