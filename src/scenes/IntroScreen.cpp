#include "IntroScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Services.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/SFML_Helper.hpp"

static constexpr float DURATION =  6.f;
static constexpr float JINGLE_TIME = 3.f;
static constexpr float TITLE_INITIAL_SCALE = 3.f;

IntroScreen::IntroScreen():
    m_elapsed(0),
    m_entities(EntityManager::getInstance()),
    m_jinglePlayed(false)
{
    m_background.setTexture(Resources::getTexture("gui/background.png"));

    sf::Texture& logo = Resources::getTexture("gui/cosmoscroll-logo.png");
    logo.setSmooth(true);
    m_title.setTexture(logo);
    m_title.setOrigin(sfh::getCenter(m_title));
    m_title.setPosition(APP_WIDTH / 2, APP_HEIGHT / 2);

    // Display a player ship instance in the intro scene
    m_spaceship = new Player();
    m_spaceship->setPosition(0, 100);

    // Allow the player ship to go beyond screen limits during the intro scene
    m_entities.resize(2000, 2000);
    m_entities.addEntity(m_spaceship);
}


void IntroScreen::onEvent(const sf::Event& event)
{
    Action::ID action = Input::feedEvent(event);
    switch (action) {
        case Action::VALIDATE:
            m_elapsed = DURATION;
            break;
        default:
            m_spaceship->onActionDown(action);
            break;
    }
}


void IntroScreen::update(float frametime)
{
    m_elapsed += frametime;
    // Play cosmoscroll jingle once
    if (!m_jinglePlayed && m_elapsed >= JINGLE_TIME) {
        m_jinglePlayed = true;
        Services::getSoundSystem().playSound("title.ogg");
    }

    m_entities.update(frametime);
    m_spaceship->move(200 * frametime, 30 * frametime);

    // Zooming
    const float time = DURATION - m_elapsed;
    const float scale = time * TITLE_INITIAL_SCALE / DURATION + 0.5f;
    m_title.setScale(scale, scale);

    // Fading
    m_title.setColor(sf::Color(255, 255, 255, (sf::Uint8) (255 * m_elapsed / DURATION)));

    if (m_elapsed >= DURATION) {
        // make entity manager ready for game use
        m_entities.clearEntities();
        Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
    }
}


void IntroScreen::draw(sf::RenderTarget& target) const
{
    target.draw(m_background);
    target.draw(m_entities);
    Game::getInstance().resetView();
    target.draw(m_title);
}
