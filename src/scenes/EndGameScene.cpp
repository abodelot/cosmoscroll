#include "EndGameScene.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Resources.hpp"
#include "core/UserSettings.hpp"
#include "core/Input.hpp"
#include "core/SoundSystem.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "utils/I18n.hpp"

// Switch to next menu after this delay
#define SCREEN_DURATION 7


EndGameScene::EndGameScene():
	m_entities(EntityManager::getInstance())
{
	m_text.setCharacterSize(40);
	m_text.setColor(sf::Color::White);
	m_text.setFont(Resources::getFont("hemi-head.ttf"));
}


void EndGameScene::OnEvent(const sf::Event& event)
{
	Input::Action action = Input::GetInstance().EventToAction(event);
	if (action == Input::ENTER)
		goNextScreen();
}


void EndGameScene::OnFocus()
{
	m_started_at.restart();

	if (m_entities.getPlayer()->isDead())
	{
		SoundSystem::GetInstance().PlaySound("game-over.ogg");
		m_text.setString(_t("endgame.game_over"));
	}
	else
	{
		// Level completed, collect $$$
		SoundSystem::GetInstance().PlaySound("end-level.ogg");
		int earned_credits = m_entities.getPlayer()->getScore();
		UserSettings::updateCredits(earned_credits);

		LevelManager& levels = LevelManager::getInstance();
		size_t current = levels.getCurrent();
		// If last level completed
		if (current == levels.getLevelCount())
		{
			m_text.setString(I18n::templatize("endgame.last_level_completed", "{credits}", earned_credits));
		}
		else
		{
			m_text.setString(I18n::templatize("endgame.level_completed", "{level}", current, "{credits}", earned_credits));
			// Unlock next level
			if (current == levels.getLastUnlocked())
			{
				levels.unlockNextLevel();
			}
		}
	}
	// Center text on screen
	float width = m_text.getLocalBounds().width;
	float height = m_text.getLocalBounds().height;
	m_text.setPosition((APP_WIDTH - width) / 2, (APP_HEIGHT - height) / 2);

	Game::getInstance().getWindow().setMouseCursorVisible(true);
	Game::getInstance().getWindow().setKeyRepeatEnabled(true);
}


void EndGameScene::Update(float frametime)
{
	Player* player = m_entities.getPlayer();
	if (player->isDead())
	{
		// Falling animation
		player->move(-100 * frametime, 100 * frametime);
		player->rotate(-60 * frametime);
	}
	ParticleSystem::GetInstance().Update(frametime);

	if (m_started_at.getElapsedTime().asSeconds() >= SCREEN_DURATION)
		goNextScreen();
}


void EndGameScene::Show(sf::RenderTarget& target) const
{
	target.draw(ControlPanel::getInstance());
	target.draw(m_entities);
	target.draw(m_text);
}


void EndGameScene::goNextScreen() const
{
	if (m_entities.GetMode() == EntityManager::MODE_STORY)
		Game::getInstance().setNextScene(Game::SC_LevelMenu);
	else
		Game::getInstance().setNextScene(Game::SC_GameOverMenu);
}
