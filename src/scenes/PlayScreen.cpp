#include "PlayScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/UserSettings.hpp"
#include "core/Services.hpp"
#include "core/Input.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"


PlayScreen::PlayScreen():
    m_entities(EntityManager::getInstance()),
    m_game(Game::getInstance()),
    m_messageSystem(Services::getMessageSystem())
{
}


void PlayScreen::onEvent(const sf::Event& event)
{
    Action::ID action = Input::feedEvent(event);
    switch (action) {
        case Action::NONE:
            break;
        case Action::PAUSE:
            m_game.setCurrentScreen(Game::SC_PauseMenu);
            break;
        default:
            m_entities.getPlayer()->onActionDown(action);
            break;
    }

    // Handle some special cases for player animations and window focus
    switch (event.type) {
        case sf::Event::KeyReleased:
            m_entities.getPlayer()->onActionUp(Input::matchKey(event.key.code));
            break;
        case sf::Event::LostFocus:
            m_game.setCurrentScreen(Game::SC_PauseMenu);
            break;
        default:
            break;
    }
}


void PlayScreen::onFocus()
{
    m_messageSystem.clear();
    m_game.getWindow().setMouseCursorVisible(false);
    m_game.getWindow().setKeyRepeatEnabled(false);
}


void PlayScreen::update(float frametime)
{
    if (m_entities.gameOverOrCompleted()) {
        m_game.setCurrentScreen(Game::SC_GameOverScreen);
    } else {
        m_entities.update(frametime);
        m_messageSystem.update(frametime);
        HUD& hud = Services::getHUD();
        hud.update(frametime);
        hud.setElapsedTime(m_entities.getTimer());
        hud.setPlayerPosition(m_entities.getPlayer()->getX());
    }
}


void PlayScreen::draw(sf::RenderTarget& target) const
{
    target.draw(m_entities);
    // Draw messages
    target.draw(m_messageSystem);
    m_game.resetView();

    target.draw(Services::getHUD());
}
