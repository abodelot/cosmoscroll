#include "IntroLevelScene.hpp"
#include "../core/Game.hpp"
#include "../core/LevelManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/MediaManager.hpp"


IntroLevelScene::IntroLevelScene()
{
	background_.SetImage(GET_IMG("background"));

	description_.SetColor(sf::Color::White);
	description_.SetFont(GET_FONT());
	description_.SetSize(30);

	title_.SetImage(GET_IMG("cosmoscroll-logo"));
	title_.SetCenter(title_.GetSize().x / 2, 0);
	title_.SetPosition(Game::WIDTH / 2, 12);
}


void IntroLevelScene::OnEvent(const sf::Event& event)
{
	switch (event.Type)
	{
		case sf::Event::KeyPressed:
		case sf::Event::JoyButtonPressed:
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		default:
			break;
	}
}


void IntroLevelScene::Show(sf::RenderTarget& target) const
{
	target.Draw(background_);
	target.Draw(title_);
	target.Draw(description_);
}


void IntroLevelScene::Poke()
{
	// get the description of the current level
	LevelManager& levels = LevelManager::GetInstance();
	int current_level = levels.GetCurrent();

	std::string content = str_sprintf(
		"Niveau %d\n\n%s\n\nEnnemis : %d",
		current_level,
		levels.GetDescription(current_level),
		levels.RemainingEntities()
	);
	str_replace(content, "\\n", "\n"); // handling line feed characters
	description_.SetText(content);
	// centered on screen
	sf::FloatRect rect = description_.GetRect();
	description_.SetPosition(
		(Game::WIDTH  - rect.GetWidth())  / 2,
		(Game::HEIGHT - rect.GetHeight()) / 2
	);
}
