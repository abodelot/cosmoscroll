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


void EndGameScene::Poke()
{
	timer_ = 0.f;

	// on ne peut pas gagner en arcade
	EntityManager::Mode mode = entities_.GetMode();
	if (mode == EntityManager::MODE_ARCADE || entities_.Count() > 1)
	{
		SoundSystem::GetInstance().PlaySound("game-over");
		info_.SetText(I18n::t("endgame.game_over"));
	}
	else
	{
		LevelManager& levels = LevelManager::GetInstance();
		int current = levels.GetCurrent();
		if (current < levels.CountLevel())
		{
			SoundSystem::GetInstance().PlaySound("end-level");

			info_.SetText(str_sprintf(I18n::t("endgame.end_level").c_str(), current));
			levels.SetCurrent(++current);

			if (current > levels.GetLastUnlocked())
			{
				levels.SetLastUnlocked(current);
#ifdef DEBUG
				printf("level %d unlocked\n", current);
#endif
			}
		}
		else // si dernier niveau du jeu
		{
			SoundSystem::GetInstance().PlaySound("end-level");

			std::wstring epic_win = str_sprintf(I18n::t("endgame.end_last_level").c_str(), current);
			info_.SetText(epic_win);
			info_.SetSize(30);
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
