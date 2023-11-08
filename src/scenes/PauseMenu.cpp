#include "PauseMenu.hpp"
#include "core/Input.hpp"
#include "core/Game.hpp"
#include "core/Services.hpp"
#include "core/SoundSystem.hpp"
#include "entities/EntityManager.hpp"
#include "utils/I18n.hpp"


PauseMenu::PauseMenu()
{
    setTitle(_t("pause.title"), 120);

    gui::VBoxLayout layout(210, 200);
    layout.add(new CosmoButton(this, _t("pause.resume")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_PlayScreen);
        Services::getSoundSystem().playMusic();
    });
    layout.add(new CosmoButton(this, _t("back_main_menu")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
    });
    layout.add(new CosmoButton(this, _t("pause.quit")))->setCallback([]() {
        Game::getInstance().quit();
    });
}


void PauseMenu::onEvent(const sf::Event& event)
{
    Action::ID action = Input::feedEvent(event);
    if (action == Action::PAUSE) {
         // Resume game
        Services::getSoundSystem().playMusic();
        Game::getInstance().setCurrentScreen(Game::SC_PlayScreen);
    } else {
        BaseMenu::onEvent(event);
    }
}


void PauseMenu::onFocus()
{
    Game::getInstance().getWindow().setMouseCursorVisible(true);
    Game::getInstance().getWindow().setKeyRepeatEnabled(true);

    Services::getSoundSystem().pauseMusic();
}


void PauseMenu::update(float frametime)
{
    Services::getParticleSystem().update(frametime);
}


void PauseMenu::draw(sf::RenderTarget& target) const
{
    target.draw(EntityManager::getInstance());
    Game::getInstance().resetView();
    target.draw(Services::getHUD());
    target.draw(getTitle());
    gui::Menu::show(target);
}

