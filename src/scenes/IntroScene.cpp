#include "IntroScene.hpp"
#include "core/Game.hpp"
#include "core/SoundSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/PlayerShip.hpp"
#include "utils/Resources.hpp"

#define DURATION    6.f
#define JINGLE_TIME 2.f
#define ZOOM_FACTOR 7


IntroScene::IntroScene() :
	entity_mgr_(EntityManager::GetInstance())
{
	background_.SetImage(Resources::GetImage("gui/background.png"));

	sf::Image& logo = Resources::GetImage("gui/cosmoscroll-logo.png");
	logo.SetSmooth(true);
	title_.SetImage(logo);
	title_.SetCenter(title_.GetSize().x / 2, title_.GetSize().y / 2);
	title_.SetPosition(Game::WIDTH / 2, Game::HEIGHT / 2);
	title_.Resize(title_.GetSize().x * ZOOM_FACTOR, title_.GetSize().y * ZOOM_FACTOR);

	// show a tempory player ship during the scene
	ship_ = new PlayerShip(sf::Vector2f(-200, 100), "player");
	entity_mgr_.SetSize(Game::WIDTH, Game::HEIGHT);
	entity_mgr_.AddEntity(ship_);

	// allow the player ship to go beyond screen limits during the intro scene
	entity_mgr_.SetSize(1000, 1000);
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
		SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("title.ogg"));
	}

	entity_mgr_.Update(frametime);
	ship_->Move(170 * frametime, 25 * frametime);
	title_.Scale(0.99, 0.99); // FIXME: dépendant des FPS
	// fading
	title_.SetColor(sf::Color(255, 255, 255,
		(sf::Uint8) (255 * elapsed_ / DURATION)));

	if (elapsed_ >= DURATION)
	{
		// make entity manager ready for game use and restore original size
		entity_mgr_.Clear();
		entity_mgr_.SetSize(Game::WIDTH, Game::HEIGHT - ControlPanel::HEIGHT);
		Game::GetInstance().SetNextScene(Game::SC_MainMenu);
	}
}


void IntroScene::Show(sf::RenderTarget& target) const
{
	target.Draw(background_);
	target.Draw(entity_mgr_);
	target.Draw(title_);
}


