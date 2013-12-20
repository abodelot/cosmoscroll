#include "InGameScene.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/UserSettings.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "core/SoundSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"


InGameScene::InGameScene():
	entities_(EntityManager::getInstance()),
	panel_(ControlPanel::getInstance())
{
}


void InGameScene::OnEvent(const sf::Event& event)
{
	static Input& input = Input::GetInstance();
	Input::Action action = input.EventToAction(event);
	switch (action)
	{
		case Input::PANEL_UP:
			setPanelOnTop(true);
			break;
		case Input::PANEL_DOWN:
			setPanelOnTop(false);
			break;
		case Input::PAUSE:
			SoundSystem::GetInstance().PauseMusic();
			Game::getInstance().setNextScene(Game::SC_PauseMenu);
			break;
		default:
			break;
	}
	entities_.getPlayer()->onEvent(event);
}


void InGameScene::OnFocus()
{
	Game::getInstance().getWindow().setMouseCursorVisible(false);
	Game::getInstance().getWindow().setKeyRepeatEnabled(false);

	setPanelOnTop(UserSettings::panel_on_top);
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


void InGameScene::setPanelOnTop(bool top)
{
	if (top)
	{
		panel_.setPosition(0, 0);
		entities_.setPosition(0, ControlPanel::HEIGHT);
	}
	else
	{
		panel_.setPosition(0, APP_HEIGHT - ControlPanel::HEIGHT);
		entities_.getInstance().setPosition(0, 0);
	}
	UserSettings::panel_on_top = top;
}
