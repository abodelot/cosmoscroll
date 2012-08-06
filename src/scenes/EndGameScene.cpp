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
	info_.setCharacterSize(40);
	info_.setColor(sf::Color::White);
	info_.setFont(GetMenuFont());
	player_dead_ = false;
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

	if (player_dead_)
	{
		PlayerShip* player = EntityManager::GetInstance().GetPlayerShip();
		player->move(0, frametime * 100);
		player->rotate(10 * frametime);
		player->UpdateSubRect(*player, frametime);
	}
}


void EndGameScene::Show(sf::RenderTarget& target) const
{
	target.draw(ControlPanel::GetInstance());
	target.draw(entities_);
	target.draw(info_);
}


void EndGameScene::OnFocus()
{
	timer_ = 0.f;

	// on ne peut pas gagner en arcade
	EntityManager::Mode mode = entities_.GetMode();
	if (mode == EntityManager::MODE_ARCADE || entities_.Count() > 1)
	{
		SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("game-over.ogg"));
		info_.setString(_t("endgame.game_over"));
		player_dead_ = true;
	}
	else
	{
		// niveau terminé
		LevelManager& levels = LevelManager::GetInstance();
		SoundSystem::GetInstance().PlaySound(Resources::getSoundBuffer("end-level.ogg"));
		int earned_credits = entities_.GetPlayerShip()->GetPoints();
		int current = levels.GetCurrent();
		// si dernier niveau du jeu
		if (current % levels.CountLevel() == 0)
		{
			std::wstring s = wstr_replace(_t("endgame.end_last_level"), L"{credits}", to_wstring(earned_credits));
			info_.setString(s);
		}
		else
		{
			std::wstring s = wstr_replace(_t("endgame.end_level"), L"{level}", to_wstring(current % levels.CountLevel()));
			wstr_self_replace(s, L"{credits}", to_wstring(earned_credits));
			info_.setString(s);
		}

		Game::GetPlayerSave().UpdateCredits(earned_credits);
		if (current == levels.GetLastUnlocked())
		{
			// nouveau niveau débloqué (si possible)
			levels.UnlockNextLevel();
		}
		player_dead_ = false;
	}

	info_.setPosition(
		(Game::WIDTH - info_.getWidth()) / 2,
		(Game::HEIGHT - info_.getHeight()) / 2
	);
}
