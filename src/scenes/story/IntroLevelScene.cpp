#include "IntroLevelScene.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/LevelManager.hpp"
#include "core/Resources.hpp"
#include "entities/EntityManager.hpp"
#include "utils/I18n.hpp"


IntroLevelScene::IntroLevelScene()
{
	m_background.setTexture(Resources::getTexture("gui/background.png"));

	m_title.setTexture(Resources::getTexture("gui/cosmoscroll-logo.png"));
	m_title.setPosition((APP_WIDTH - m_title.getTextureRect().width) / 2, 20);

	const sf::Font& font = Resources::getFont("hemi-head.ttf");
	m_level_text.setColor(sf::Color::White);
	m_level_text.setStyle(sf::Text::Bold);
	m_level_text.setFont(font);
	m_level_text.setCharacterSize(50);
	m_level_text.setPosition(60, 120);

	m_quote_text.setColor(sf::Color(230, 230, 230));
	m_quote_text.setStyle(sf::Text::Italic);
	m_quote_text.setFont(font);
	m_quote_text.setCharacterSize(35);
	m_quote_text.setPosition(60, 210);
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
	target.draw(m_background);
	target.draw(m_title);
	target.draw(m_level_text);
	target.draw(m_quote_text);
}


void IntroLevelScene::OnFocus()
{
	LevelManager& levels = LevelManager::getInstance();
	size_t current_level = levels.getCurrent();

#ifdef DEBUG
	printf("level %u (%s)\n", current_level, levels.getMusicName());
	printf(" - total points: %d\n", levels.getTotalPoints());
	printf(" - entities:     %d\n", levels.getSpawnQueueSize());
	printf(" - duration:     %02d:%02d\n", (int) levels.getDuration() / 60, (int) levels.getDuration() % 60);
#endif
	// Get current level's attributes
	m_level_text.setString(I18n::templatize("menu.story.level", "{number}", current_level));
	m_quote_text.setString(str_replace<std::string>(levels.getQuote(), "\\n", "\n"));
}
