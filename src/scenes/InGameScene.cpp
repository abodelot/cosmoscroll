#include "InGameScene.hpp"
#include "../core/Game.hpp"
#include "../core/Input.hpp"
#include "../core/ControlPanel.hpp"
#include "../entities/EntityManager.hpp"


InGameScene::InGameScene():
	entities_(EntityManager::GetInstance()),
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
			panel_.SetY(0);
			entities_.SetY(ControlPanel::HEIGHT);
			break;
		case Input::PANEL_DOWN:
			panel_.SetY(Game::HEIGHT - ControlPanel::HEIGHT);
			entities_.SetY(0);
			break;
		case Input::PAUSE:
			Game::GetInstance().SetNextScene(Game::SC_PauseMenu);
			break;
		default:
			entities_.HandleAction(action);
			break;
	}
}


void InGameScene::Update(float frametime)
{
	if (entities_.IsGameOver())
	{
		Game::GetInstance().SetNextScene(Game::SC_EndGameScene);
	}
	else
	{
		entities_.Update(frametime);
		panel_.SetTimer(entities_.GetTimer());
	}
}


void InGameScene::Show(sf::RenderTarget& target) const
{
	target.Draw(entities_);
	target.Draw(panel_);
}


void InGameScene::OnFocus()
{
	Game::GetInstance().GetApp().ShowMouseCursor(false);
}
