#include "InGameScene.hpp"
#include "core/Game.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "core/SoundSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"


InGameScene::InGameScene():
	entities_(EntityManager::getInstance()),
	panel_(ControlPanel::GetInstance())
{
}


void InGameScene::OnEvent(const sf::Event& event)
{
	static Input& input = Input::GetInstance();
	Input::Action action = input.EventToAction(event);
	switch (action)
	{
		case Input::PANEL_UP:
			panel_.setPosition(0, 0);
			entities_.setPosition(0, ControlPanel::HEIGHT);
			break;
		case Input::PANEL_DOWN:
			panel_.setPosition(0, Game::HEIGHT - ControlPanel::HEIGHT);
			entities_.setPosition(0, 0);
			break;
		case Input::PAUSE:
			SoundSystem::GetInstance().PauseMusic();
			Game::getInstance().setNextScene(Game::SC_PauseMenu);
			break;
		default:
			break;
	}
	entities_.GetPlayerShip()->onEvent(event);
}


void InGameScene::Update(float frametime)
{
	if (entities_.IsGameOver())
	{
		SoundSystem::GetInstance().StopMusic();
		Game::getInstance().setNextScene(Game::SC_EndGameScene);
	}
	else
	{
		entities_.Update(frametime);
		panel_.Update(frametime);
		panel_.SetTimer(entities_.GetTimer());
	}
}


void InGameScene::Show(sf::RenderTarget& target) const
{
	target.draw(entities_);
	target.draw(panel_);
}


void InGameScene::OnFocus()
{
	Game::getInstance().getWindow().setMouseCursorVisible(false);
	Game::getInstance().getWindow().setKeyRepeatEnabled(false);
}
