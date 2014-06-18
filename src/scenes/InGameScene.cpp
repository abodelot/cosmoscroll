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
	m_entities(EntityManager::getInstance()),
	m_panel(ControlPanel::getInstance())
{
}


void InGameScene::OnEvent(const sf::Event& event)
{
	Action::ID action = Input::feedEvent(event);
	switch (action)
	{
		case Action::PANEL_UP:
			setPanelOnTop(true);
			break;
		case Action::PANEL_DOWN:
			setPanelOnTop(false);
			break;
		case Action::PAUSE:
			SoundSystem::pauseMusic();
			Game::getInstance().setNextScene(Game::SC_PauseMenu);
			break;
		default:
			break;
	}
	m_entities.getPlayer()->onEvent(event);
}


void InGameScene::OnFocus()
{
	Game::getInstance().getWindow().setMouseCursorVisible(false);
	Game::getInstance().getWindow().setKeyRepeatEnabled(false);

	setPanelOnTop(UserSettings::panel_on_top);
}


void InGameScene::Update(float frametime)
{
	if (m_entities.spawnBadGuys())
	{
		SoundSystem::stopMusic();
		Game::getInstance().setNextScene(Game::SC_EndGameScene);
	}
	else
	{
		m_entities.update(frametime);
		m_panel.Update(frametime);
		m_panel.SetTimer(m_entities.GetTimer());
	}
}


void InGameScene::Show(sf::RenderTarget& target) const
{
	target.draw(m_entities);
	target.draw(m_panel);
}


void InGameScene::setPanelOnTop(bool top)
{
	if (top)
	{
		m_panel.setPosition(0, 0);
		m_entities.setPosition(0, ControlPanel::HEIGHT);
	}
	else
	{
		m_panel.setPosition(0, APP_HEIGHT - ControlPanel::HEIGHT);
		m_entities.getInstance().setPosition(0, 0);
	}
	UserSettings::panel_on_top = top;
}
