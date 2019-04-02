#include "InfinityModeMenu.hpp"
#include "utils/I18n.hpp"
#include "core/Game.hpp"
#include "core/UserSettings.hpp"
#include "core/ControlPanel.hpp"
#include "entities/EntityManager.hpp"


InfinityModeMenu::InfinityModeMenu()
{
    setTitle(_t("infinity.title"));

    gui::VBoxLayout layout(210, 120);
    layout.Add(new CosmoButton(this, _t("infinity.play")))->SetCallbackID(1);
    layout.Add(new CosmoButton(this, _t("infinity.leaderboard")))->SetCallbackID(2);
    layout.Add(new CosmoButton(this, _t("back_main_menu")))->SetCallbackID(3);
}


void InfinityModeMenu::EventCallback(int id)
{
    Game& game = Game::getInstance();
    switch (id)
    {
        case 1:
            EntityManager::getInstance().setMode(EntityManager::INFINITY_MODE);
            ControlPanel::getInstance().setHighscore(UserSettings::getHighscore());
            game.setCurrentScreen(Game::SC_PlayScreen);
            break;
        case 2:
            game.setCurrentScreen(Game::SC_LeaderboardMenu);
            break;
        case 3:
            game.setCurrentScreen(Game::SC_MainMenu);
            break;
    }
}
