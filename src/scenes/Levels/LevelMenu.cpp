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

    m_opt_levels = new gui::OptionList<int>(this);
    form.addRow(_t("levels.select"), m_opt_levels);

    m_lab_progresion = new gui::Label(this, "");
    form.addRow(_t("levels.progression"), m_lab_progresion);

    gui::VBoxLayout layout(210, 240);
    // Play selected level
    layout.Add(new CosmoButton(this, _t("levels.play")))->setCallback([this]() {
        int selected_level = m_opt_levels->GetSelectedValue();
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
        ControlPanel::getInstance().setGameInfo(
            I18n::templatize("panel.level", "{level}", selected_level)
        );
        // Init entity manager
        EntityManager::getInstance().initialize();
        Game::getInstance().setCurrentScreen(Game::SC_PlayScreen);
    });

    // Go to armory menu
    layout.Add(new CosmoButton(this, _t("levels.armory")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_ArmoryMenu);
    });

    // Back to main menu
    layout.Add(new CosmoButton(this, _t("back_main_menu")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
    });
}


void LevelMenu::onFocus()
{
    m_credits->setCredits(UserSettings::getCredits());
    size_t nb_levels = m_levels.getLevelCount();
    size_t current = m_levels.getCurrent();
    size_t last_unlocked = m_levels.getLastUnlocked();

    std::string progression = std::to_string(last_unlocked) + "/"
        + std::to_string(nb_levels);
    m_lab_progresion->setString(progression);

    // option widget
    m_opt_levels->Clear();
    for (size_t i = 1; i <= nb_levels; ++i)
    {
        bool activable = i <= last_unlocked;
        if (activable)
        {
            m_opt_levels->Add(std::to_string(i), i);
        }
    }
    m_opt_levels->Select(current - 1);
}
