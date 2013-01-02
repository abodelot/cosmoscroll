#include "IntroLevelScene.hpp"
#include "core/Game.hpp"
#include "core/LevelManager.hpp"
#include "utils/StringUtils.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "utils/I18n.hpp"


IntroLevelScene::IntroLevelScene()
{
	background_.setTexture(Resources::getTexture("gui/background.png"));

	description_.setColor(sf::Color::White);
	description_.setFont(GetMenuFont());
	description_.setCharacterSize(35);

	title_.setTexture(Resources::getTexture("gui/cosmoscroll-logo.png"));
	title_.setOrigin(title_.getWidth() / 2, 0);
	title_.setPosition(Game::WIDTH / 2, 20);
}


void IntroLevelScene::OnEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			if (event.key.code != sf::Keyboard::Escape)
			{
				EntityManager::getInstance().InitMode(EntityManager::MODE_STORY);
				Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			}
			else
			{
				Game::GetInstance().SetNextScene(Game::SC_LevelMenu);
			}
			break;
		case sf::Event::JoystickButtonPressed:
			EntityManager::getInstance().InitMode(EntityManager::MODE_STORY);
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		default:
			break;
	}
}


void IntroLevelScene::Show(sf::RenderTarget& target) const
{
	target.draw(background_);
	target.draw(title_);
	target.draw(description_);
}


void IntroLevelScene::OnFocus()
{
	// get the description of the current level
	LevelManager& levels = LevelManager::GetInstance();
	int current_level = levels.GetCurrent();

	printf("level %d\n", current_level);
	printf("  - entities: %d\n", levels.RemainingEntities());
	printf("  - total points: %d\n", levels.GetTotalPoints());
	printf("  - duration: %d\" %02d'\n", levels.GetDuration() / 60, levels.GetDuration() % 60);

	std::wstring intro = _t("menu.story.intro");
	if (current_level > levels.CountLevel())
	{
		current_level -= levels.CountLevel();
	}
	wstr_self_replace(intro, L"{level}", to_wstring(current_level));
	wstr_self_replace(intro, L"{description}", I18n::DecodeUTF8(levels.GetDescription()));
	wstr_self_replace(intro, L"{count}", to_wstring(levels.RemainingEntities()));
	wstr_self_replace(intro, L"\\n", L"\n");
	description_.setString(intro);

	// centered on screen
	description_.setPosition(
		(Game::WIDTH  - description_.getWidth())  / 2,
		(Game::HEIGHT - description_.getHeight()) / 2
	);
}
