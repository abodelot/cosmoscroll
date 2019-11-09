#include "MainMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"


MainMenu::MainMenu()
{
    m_title.setTexture(Resources::getTexture("gui/cosmoscroll-logo.png"));
    m_title.setPosition((APP_WIDTH - m_title.getLocalBounds().width) / 2, 12);

    gui::VBoxLayout layout(210, 120);
    layout.SetSpacing(0, 14);

    layout.Add(new CosmoButton(this, _t("main.levels")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_LevelMenu);
    });
    layout.Add(new CosmoButton(this, _t("main.options")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_OptionMenu);
    });
    layout.Add(new CosmoButton(this, _t("main.about")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_AboutMenu);
    });
    layout.Add(new CosmoButton(this, _t("main.quit")))->setCallback([]() {
        Game::getInstance().quit();
    });
}


void MainMenu::draw(sf::RenderTarget& target) const
{
    BaseMenu::draw(target);
    target.draw(m_title);
}
