#include "IntroScreen.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/SFML_Helper.hpp"

#define DURATION    6.f
#define JINGLE_TIME 3.f
#define ZOOM_FACTOR 3


IntroScreen::IntroScreen():
	m_entities(EntityManager::getInstance())
{
	m_background.setTexture(Resources::getTexture("gui/background.png"));

	sf::Texture& logo = Resources::getTexture("gui/cosmoscroll-logo.png");
	logo.setSmooth(true);
	m_title.setTexture(logo);
	m_title.setOrigin(sfh::getCenter(m_title));
	m_title.setPosition(APP_WIDTH / 2, APP_HEIGHT / 2);

	// Display a player ship instance in the intro scene
	ship_ = new Player();
	ship_->setPosition(0, 100);

	// Allow the player ship to go beyond screen limits during the intro scene
	m_entities.setPosition(-500, 0);
	m_entities.resize(2000, 2000);
	m_entities.addEntity(ship_);

	elapsed_ = 0.f;
}


void IntroScreen::OnEvent(const sf::Event& event)
{
	Action::ID action = Input::feedEvent(event);
	switch (action)
	{
		case Action::VALIDATE:
			elapsed_ = DURATION;
			break;
		default:
			ship_->onActionDown(action);
			break;
	}
}


void IntroScreen::Update(float frametime)
{
	static bool jingle_played = false;

	elapsed_ += frametime;
	// play cosmoscroll jingle once
	if (!jingle_played && elapsed_ >= JINGLE_TIME)
	{
		jingle_played = true;
		SoundSystem::playSound("title.ogg");
	}

	m_entities.update(frametime);
	ship_->move(200 * frametime, 30 * frametime);

	// Zooming
	sf::IntRect rect = m_title.getTextureRect();
	float width = rect.width * ZOOM_FACTOR * (DURATION - elapsed_) / DURATION;
	float height = rect.height * ZOOM_FACTOR * (DURATION - elapsed_) / DURATION;
	sfh::resize(m_title, width, height);

	// Fading
	m_title.setColor(sf::Color(255, 255, 255, (sf::Uint8) (255 * elapsed_ / DURATION)));

	if (elapsed_ >= DURATION)
	{
		// make entity manager ready for game use and restore original size
		m_entities.clearEntities();
		m_entities.resize(APP_WIDTH, APP_HEIGHT - ControlPanel::HEIGHT);
		Game::getInstance().setNextScene(Game::SC_MainMenu);
	}
}


void IntroScreen::Show(sf::RenderTarget& target) const
{
	target.draw(m_background);
	target.draw(m_entities);
	target.draw(m_title);
}


