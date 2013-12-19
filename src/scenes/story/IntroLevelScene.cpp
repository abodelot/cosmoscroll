#include "IntroLevelScene.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/LevelManager.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "utils/I18n.hpp"


IntroLevelScene::IntroLevelScene()
{
	background_.setTexture(Resources::getTexture("gui/background.png"));

	description_.setColor(sf::Color::White);
	description_.setFont(Resources::getFont("hemi-head.ttf"));
	description_.setCharacterSize(35);

	title_.setTexture(Resources::getTexture("gui/cosmoscroll-logo.png"));
	title_.setOrigin(title_.getWidth() / 2, 0);
	title_.setPosition(APP_WIDTH / 2, 20);
}


void IntroLevelScene::OnEvent(const sf::Event& event)
{
	switch (event.type)
	{
		case sf::Event::KeyPressed:
			if (event.key.code != sf::Keyboard::Escape)
			{
				EntityManager::getInstance().InitMode(EntityManager::MODE_STORY);
				Game::getInstance().setNextScene(Game::SC_InGameScene);
			}
			else
			{
				Game::getInstance().setNextScene(Game::SC_LevelMenu);
			}
			break;
		case sf::Event::JoystickButtonPressed:
			EntityManager::getInstance().InitMode(EntityManager::MODE_STORY);
			Game::getInstance().setNextScene(Game::SC_InGameScene);
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
	LevelManager& levels = LevelManager::getInstance();
	size_t current_level = levels.getCurrent();

#ifdef DEBUG
	printf("level %u\n", current_level);
	printf(" - entities:     %d\n", levels.getSpawnQueueSize());
	printf(" - total points: %d\n", levels.getTotalPoints());
	printf(" - duration:     %02d:%02d\n", (int) levels.getDuration() / 60, (int) levels.getDuration() % 60);
#endif
	// Get current level's attributes
	std::wstring intro = _t("menu.story.intro");
	wstr_self_replace(intro, L"{level}", std::to_wstring(current_level));
	wstr_self_replace(intro, L"{description}", decode_utf8(levels.getDescription()));
	wstr_self_replace(intro, L"{count}", std::to_wstring(levels.getSpawnQueueSize()));
	wstr_self_replace(intro, L"\\n", L"\n");
	description_.setString(intro);

	// Center description on screen
	description_.setPosition(
		(APP_WIDTH  - description_.getWidth())  / 2,
		(APP_HEIGHT - description_.getHeight()) / 2
	);
}
