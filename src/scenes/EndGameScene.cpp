#include "EndGameScene.hpp"
#include "core/Game.hpp"
#include "core/Input.hpp"
#include "core/SoundSystem.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "entities/EntityManager.hpp"
#include "entities/PlayerShip.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"

#define DURATION 7


EndGameScene::EndGameScene():
	entities_(EntityManager::GetInstance())
{
	timer_ = 0.f;
	info_.SetSize(40);
	info_.SetColor(sf::Color::White);
	info_.SetFont(GetMenuFont());
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
		SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("game-over.ogg"));
		info_.SetText(_t("endgame.game_over"));
	}
	else
	{
		// niveau terminé
		LevelManager& levels = LevelManager::GetInstance();
		SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("end-level.ogg"));
		int earned_credits = entities_.GetPlayerShip()->GetPoints();
		int current = levels.GetCurrent();
		// si dernier niveau du jeu
		if (current % levels.CountLevel() == 0)
		{
			std::wstring s = wstr_replace(_t("endgame.end_last_level"), L"{credits}", to_wstring(earned_credits));
			info_.SetText(s);
		}
		else
		{
			std::wstring s = wstr_replace(_t("endgame.end_level"), L"{level}", to_wstring(current % levels.CountLevel()));
			wstr_self_replace(s, L"{credits}", to_wstring(earned_credits));
			info_.SetText(s);
		}

		Game::GetPlayerSave().UpdateCredits(earned_credits);
		if (current == levels.GetLastUnlocked())
		{
			// nouveau niveau débloqué (si possible)
			levels.UnlockNextLevel();
		}
	}

	sf::FloatRect rect = info_.GetRect();
	info_.SetPosition(
		(Game::WIDTH - rect.GetWidth()) / 2,
		(Game::HEIGHT - rect.GetHeight()) / 2
	);
}
