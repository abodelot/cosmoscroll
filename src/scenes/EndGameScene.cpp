#include "EndGameScene.hpp"
#include "../core/Game.hpp"
#include "../core/Input.hpp"
#include "../core/SoundSystem.hpp"
#include "../core/LevelManager.hpp"
#include "../core/ControlPanel.hpp"
#include "../entities/EntityManager.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"

#define DURATION 7


EndGameScene::EndGameScene():
	entities_(EntityManager::GetInstance())
{
	timer_ = 0.f;
	info_.SetSize(70);
	info_.SetColor(sf::Color::White);
	info_.SetFont(GET_FONT());
}


void EndGameScene::OnEvent(const sf::Event& event)
{
	Input::Action action = Input::GetInstance().EventToAction(event);
	if (action == Input::ENTER)
	{
		timer_ = DURATION;
	}
}


void EndGameScene::Update(float frametime)
{
	timer_ += frametime;
	ParticleSystem::GetInstance().Update(frametime);
	if (timer_ >= DURATION)
	{
		Game::Scene next = entities_.GetMode() == EntityManager::MODE_STORY ?
			Game::SC_LevelMenu : Game::SC_GameOverMenu;
		Game::GetInstance().SetNextScene(next);
	}
}


void EndGameScene::Show(sf::RenderTarget& target) const
{
	target.Draw(ControlPanel::GetInstance());
	target.Draw(entities_);
	target.Draw(info_);
}


void EndGameScene::OnFocus()
{
	timer_ = 0.f;

	// on ne peut pas gagner en arcade
	EntityManager::Mode mode = entities_.GetMode();
	if (mode == EntityManager::MODE_ARCADE || entities_.Count() > 1)
	{
		SoundSystem::GetInstance().PlaySound("game-over");
		info_.SetText(_t("endgame.game_over"));
	}
	else
	{
		// niveau terminé
		LevelManager& levels = LevelManager::GetInstance();
		SoundSystem::GetInstance().PlaySound("end-level");
		int current = levels.GetCurrent();
		if (current < levels.CountLevel())
		{
			std::wstring s = wstr_replace(_t("endgame.end_level"), L"{level}", to_wstring(current));
			info_.SetText(s);
		}
		else // si dernier niveau du jeu
		{
			std::wstring s = wstr_replace(_t("endgame.end_last_level"), L"{count}", to_wstring(current));
			info_.SetText(s);
			info_.SetSize(30);
		}

		if (current == levels.GetLastUnlocked())
		{
			// nouveau niveau débloqué
			current = levels.UnlockNextLevel();
		}
	}

	sf::FloatRect rect = info_.GetRect();
	info_.SetPosition(
		(Game::WIDTH - rect.GetWidth()) / 2,
		(Game::HEIGHT - rect.GetHeight()) / 2
	);

	// entitymanager_.Count() > 1 && mode_ != STORY2X) ||
	//	(entitymanager_.Count() > 2 && mode_ == STORY2X))
}
