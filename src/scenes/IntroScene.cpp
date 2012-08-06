#include "IntroScene.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/PlayerShip.hpp"
#include "core/Resources.hpp"

#define DURATION    6.f
#define JINGLE_TIME 2.f
#define ZOOM_FACTOR 7


IntroScene::IntroScene() :
	entity_mgr_(EntityManager::GetInstance())
{
	background_.setTexture(Resources::getTexture("gui/background.png"));

	sf::Texture& logo = Resources::getTexture("gui/cosmoscroll-logo.png");
	logo.setSmooth(true);
	title_.setTexture(logo);
	title_.setOrigin(title_.getCenter());
	title_.setPosition(Game::WIDTH / 2, Game::HEIGHT / 2);
	title_.resize(title_.getWidth() * ZOOM_FACTOR, title_.getHeight() * ZOOM_FACTOR);

	// show a tempory player ship during the scene
	ship_ = new PlayerShip(sf::Vector2f(-200, 100), "player");

	// allow the player ship to go beyond screen limits during the intro scene
	entity_mgr_.resize(1000, 1000);
	entity_mgr_.AddEntity(ship_);

	elapsed_ = 0.f;
}


void IntroScene::OnEvent(const sf::Event& event)
{
	Input::Action action = Input::GetInstance().EventToAction(event);
	switch (action)
	{
		case Input::ENTER:
		case Input::PAUSE:
			elapsed_ = DURATION;
			break;
		default:
			ship_->HandleAction(action);
			break;
	}
}


void IntroScene::Update(float frametime)
{
	static bool jingle_played = false;

	elapsed_ += frametime;
	// play cosmoscroll jingle once
	if (!jingle_played && elapsed_ >= JINGLE_TIME)
	{
		jingle_played = true;
		SoundSystem::GetInstance().PlaySound("title.ogg");
	}

	entity_mgr_.Update(frametime);
	ship_->move(170 * frametime, 25 * frametime);
	title_.scale(0.99, 0.99); // FIXME: make this FPS rate independant
	// fading
	title_.setColor(sf::Color(255, 255, 255,
		(sf::Uint8) (255 * elapsed_ / DURATION)));

	if (elapsed_ >= DURATION)
	{
		// make entity manager ready for game use and restore original size
		entity_mgr_.Clear();
		entity_mgr_.resize(Game::WIDTH, Game::HEIGHT - ControlPanel::HEIGHT);
		Game::GetInstance().SetNextScene(Game::SC_MainMenu);
	}
}


void IntroScene::Show(sf::RenderTarget& target) const
{
	target.draw(background_);
	target.draw(entity_mgr_);
	target.draw(title_);
}


