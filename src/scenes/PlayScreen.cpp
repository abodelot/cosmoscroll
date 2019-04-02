#include "PlayScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/UserSettings.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"


PlayScreen::PlayScreen():
    m_entities(EntityManager::getInstance()),
    m_panel(ControlPanel::getInstance())
{
}


void PlayScreen::onEvent(const sf::Event& event)
{
    Action::ID action = Input::feedEvent(event);
    switch (action)
    {
        case Action::NONE:
            break;
        case Action::PANEL_UP:
            setPanelOnTop(true);
            break;
        case Action::PANEL_DOWN:
            setPanelOnTop(false);
            break;
        case Action::PAUSE:
            Game::getInstance().setCurrentScreen(Game::SC_PauseMenu);
            break;
        default:
            m_entities.getPlayer()->onActionDown(action);
            break;
    }

    // Handle some special cases for player animations and window focus
    switch (event.type)
    {
        case sf::Event::KeyReleased:
            m_entities.getPlayer()->onActionUp(Input::matchKey(event.key.code));
            break;
        case sf::Event::LostFocus:
            Game::getInstance().setCurrentScreen(Game::SC_PauseMenu);
            break;
        default:
            break;
    }
}


void PlayScreen::onFocus()
{
    Game::getInstance().getWindow().setMouseCursorVisible(false);
    Game::getInstance().getWindow().setKeyRepeatEnabled(false);

    setPanelOnTop(UserSettings::panel_on_top);
}


void PlayScreen::update(float frametime)
{
    if (m_entities.spawnBadGuys())
    {
        Game::getInstance().setCurrentScreen(Game::SC_GameOverScreen);
    }
    else
    {
        m_entities.update(frametime);
        m_panel.update(frametime);
        m_panel.setElapsedTime(m_entities.getTimer());
    }
}


void PlayScreen::draw(sf::RenderTarget& target) const
{
    target.draw(m_entities);
    target.draw(m_panel);
}


void PlayScreen::setPanelOnTop(bool top)
{
    if (top)
    {
        m_panel.setPosition(0, 0);
        m_entities.setPosition(0, ControlPanel::HEIGHT);
    }
    else
    {
        m_panel.setPosition(0, APP_HEIGHT - ControlPanel::HEIGHT);
        m_entities.getInstance().setPosition(0, 0);
    }
    UserSettings::panel_on_top = top;
}
