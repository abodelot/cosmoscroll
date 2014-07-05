#include "IntroScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/SFML_Helper.hpp"

const float DURATION =  6.f;
const float JINGLE_TIME = 3.f;
const float ZOOM_FACTOR = 3.f;


IntroScreen::IntroScreen():
	m_elapsed(0),
	m_entities(EntityManager::getInstance())
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
	m_entities.setPosition(-500, 0);
	m_entities.resize(2000, 2000);
	m_entities.addEntity(m_spaceship);
}


void IntroScreen::onEvent(const sf::Event& event)
{
	Action::ID action = Input::feedEvent(event);
	switch (action)
	{
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
	static bool jingle_played = false;

	m_elapsed += frametime;
	// play cosmoscroll jingle once
	if (!jingle_played && m_elapsed >= JINGLE_TIME)
	{
		jingle_played = true;
		SoundSystem::playSound("title.ogg");
	}

	m_entities.update(frametime);
	m_spaceship->move(200 * frametime, 30 * frametime);

	// Zooming
	sf::IntRect rect = m_title.getTextureRect();
	float width = rect.width * ZOOM_FACTOR * (DURATION - m_elapsed) / DURATION;
	float height = rect.height * ZOOM_FACTOR * (DURATION - m_elapsed) / DURATION;
	sfh::resize(m_title, width, height);

	// Fading
	m_title.setColor(sf::Color(255, 255, 255, (sf::Uint8) (255 * m_elapsed / DURATION)));

	if (m_elapsed >= DURATION)
	{
		// make entity manager ready for game use and restore original size
		m_entities.clearEntities();
		m_entities.resize(APP_WIDTH, APP_HEIGHT - ControlPanel::HEIGHT);
		Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
	}
}


void IntroScreen::draw(sf::RenderTarget& target) const
{
	target.draw(m_background);
	target.draw(m_entities);
	target.draw(m_title);
}


