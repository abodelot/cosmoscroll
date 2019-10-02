#include "GameOverScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Resources.hpp"
#include "core/UserSettings.hpp"
#include "core/Input.hpp"
#include "core/SoundSystem.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/I18n.hpp"

// Switch to next menu after this delay
const float SCREEN_DURATION = 7.f;


GameOverScreen::GameOverScreen():
    m_entities(EntityManager::getInstance()),
    m_panel(ControlPanel::getInstance())
{
    m_text.setCharacterSize(40);
    m_text.setFillColor(sf::Color::White);
    m_text.setFont(Resources::getFont("hemi-head.ttf"));
}


void GameOverScreen::onEvent(const sf::Event& event)
{
    Action::ID action = Input::feedEvent(event);
    if (action == Action::VALIDATE)
        Game::getInstance().setCurrentScreen(Game::SC_LevelMenu);
}


void GameOverScreen::onFocus()
{
    SoundSystem::stopMusic();
    m_started_at.restart();

    if (m_entities.getPlayer()->isDead())
    {
        SoundSystem::playSound("game-over.ogg");
        m_text.setString(_t("gameover.title"));
    }
    else
    {
        // Level completed, collect $$$
        SoundSystem::playSound("end-level.ogg");
        int earned_credits = m_entities.getPlayer()->getScore();
        UserSettings::updateCredits(earned_credits);

        LevelManager& levels = LevelManager::getInstance();
        size_t current = levels.getCurrent();
        // If last level completed
        if (current == levels.getLevelCount())
        {
            m_text.setString(I18n::templatize("gameover.last_level_completed", "{credits}", earned_credits));
        }
        else
        {
            m_text.setString(I18n::templatize("gameover.level_completed", "{level}", current, "{credits}", earned_credits));
            // Unlock next level
            if (current == levels.getLastUnlocked())
            {
                levels.unlockNextLevel();
            }
        }
    }
    // Center text on screen
    float width = m_text.getLocalBounds().width;
    float height = m_text.getLocalBounds().height;
    m_text.setPosition((APP_WIDTH - width) / 2, (APP_HEIGHT - height) / 2);

    Game::getInstance().getWindow().setMouseCursorVisible(true);
    Game::getInstance().getWindow().setKeyRepeatEnabled(true);
}


void GameOverScreen::update(float frametime)
{
    Player* player = m_entities.getPlayer();
    if (player->isDead())
    {
        // Falling animation
        player->move(-100 * frametime, 100 * frametime);
        player->rotate(-60 * frametime);
    }
    ParticleSystem::getInstance().update(frametime);

    if (m_started_at.getElapsedTime().asSeconds() >= SCREEN_DURATION)
        Game::getInstance().setCurrentScreen(Game::SC_LevelMenu);
}


void GameOverScreen::draw(sf::RenderTarget& target) const
{
    target.draw(m_panel);
    target.draw(m_entities);
    target.draw(m_text);
}
