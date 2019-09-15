#include "LevelMenu.hpp"
#include "core/Game.hpp"
#include "core/UserSettings.hpp"
#include "core/Services.hpp"
#include "utils/I18n.hpp"


LevelMenu::LevelMenu()
{
    setTitle(_t("levels.title"));
    m_credits = new CreditCounterWidget(this);

    gui::FormLayout form(90, 120);
    form.setSpacing(10, 16);

    m_optLevels = new gui::OptionList<int>(this);
    form.addRow(_t("levels.select"), m_optLevels);

    m_labProgresion = new gui::Label(this, "");
    form.addRow(_t("levels.progression"), m_labProgresion);

    gui::VBoxLayout layout(210, 240);
    // Play selected level
    layout.add(new CosmoButton(this, _t("levels.play")))->setCallback([this]() {
        const int selectedLevel = m_optLevels->getSelectedValue();
        // Parse selected level
        LevelParser& levelParser = Services::getLevelParser();
        levelParser.setCurrent(selectedLevel);

        printf("-> level %u (map: %s, music: %s)\n", selectedLevel, levelParser.getTilemapPath(), levelParser.getMusicName());

        // Init HUD with level number
        Services::getHUD().setLevelNumber(selectedLevel);

        // Init entity manager
        EntityManager::getInstance().initializeLevel();
        Game::getInstance().setCurrentScreen(Game::SC_PlayScreen);
    });

    // Go to armory menu
    layout.add(new CosmoButton(this, _t("levels.armory")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_ArmoryMenu);
    });

    // Back to main menu
    layout.add(new CosmoButton(this, _t("back_main_menu")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
    });
}


void LevelMenu::onFocus()
{
    m_credits->setCredits(UserSettings::getCredits());
    LevelParser& levelParser = Services::getLevelParser();
    size_t nb_levels = levelParser.getLevelCount();
    size_t current = levelParser.getCurrent();
    size_t last_unlocked = levelParser.getLastUnlocked();

    std::string progression = std::to_string(last_unlocked) + "/"
        + std::to_string(nb_levels);
    m_labProgresion->setString(progression);

    // option widget
    m_optLevels->clear();
    for (size_t i = 1; i <= nb_levels; ++i) {
        bool activable = i <= last_unlocked;
        if (activable) {
            m_optLevels->add(std::to_string(i), i);
        }
    }
    m_optLevels->select(current - 1);
}
