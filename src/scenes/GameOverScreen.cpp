#include "GameOverScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Resources.hpp"
#include "core/UserSettings.hpp"
#include "core/Input.hpp"
#include "core/SoundSystem.hpp"
#include "core/Services.hpp"
#include "core/Services.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"

// Switch to next menu after this delay
constexpr float SCREEN_DURATION = 7.f;


GameOverScreen::GameOverScreen():
    m_entities(EntityManager::getInstance())
{
    m_text.setCharacterSize(40);
    m_text.setFillColor(sf::Color::White);
    m_text.setOutlineThickness(1.f);
    m_text.setOutlineColor(sf::Color::Black);
    m_text.setFont(Resources::getFont("hemi-head.ttf"));
}


void GameOverScreen::onEvent(const sf::Event& event)
{
    Action::ID action = Input::feedEvent(event);
    if (action == Action::VALIDATE) {
        Game::getInstance().setCurrentScreen(Game::SC_LevelMenu);
    }
}


void GameOverScreen::onFocus()
{
    Services::getSoundSystem().stopMusic();
    m_startedAt.restart();

    if (m_entities.getPlayer()->isDead()) {
        Services::getSoundSystem().playSound("game-over.ogg");
        m_text.setString(_t("gameover.title"));
    } else {
        // Level completed, collect $$$
        Services::getSoundSystem().playSound("end-level.ogg");
        int earned_credits = m_entities.getPlayer()->getScore();
        UserSettings::updateCredits(earned_credits);

        LevelParser& levelParser = Services::getLevelParser();
        size_t current = levelParser.getCurrent();
        // If last level completed
        if (current == levelParser.getLevelCount()) {
            m_text.setString(_t("gameover.last_level_completed", "{credits}", earned_credits));
        } else {
            m_text.setString(_t("gameover.level_completed", "{level}", current, "{credits}", earned_credits));
            // Unlock next level
            if (current == levelParser.getLastUnlocked()) {
                levelParser.unlockNextLevel();
            }
        }
    }
    // Center text on screen
    float width = m_text.getLocalBounds().width;
    float height = m_text.getLocalBounds().height;
    m_text.setPosition((APP_WIDTH - width) / 2, (APP_HEIGHT - height) / 2);

    sf::RenderWindow& window = Game::getInstance().getWindow();
    window.setMouseCursorVisible(true);
    window.setKeyRepeatEnabled(true);
}


void GameOverScreen::update(float frametime)
{
    Services::getParticleSystem().update(frametime);

    if (m_startedAt.getElapsedTime().asSeconds() >= SCREEN_DURATION) {
        Game::getInstance().setCurrentScreen(Game::SC_LevelMenu);
    }
}


void GameOverScreen::draw(sf::RenderTarget& target) const
{
    target.draw(m_entities);
    Game::getInstance().resetView();
    target.draw(Services::getHUD());
    target.draw(m_text);
}
