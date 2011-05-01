#include "IntroLevelScene.hpp"
#include "core/Game.hpp"
#include "core/LevelManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/MediaManager.hpp"
#include "utils/I18n.hpp"


IntroLevelScene::IntroLevelScene()
{
	background_.SetImage(GET_IMG("gui/background"));

	description_.SetColor(sf::Color::White);
	description_.SetFont(GET_FONT());
	description_.SetSize(35);

	title_.SetImage(GET_IMG("gui/cosmoscroll-logo"));
	title_.SetCenter(title_.GetSize().x / 2, 0);
	title_.SetPosition(Game::WIDTH / 2, 20);
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


void IntroLevelScene::OnFocus()
{
	// get the description of the current level
	LevelManager& levels = LevelManager::GetInstance();
	int current_level = levels.GetCurrent();

	printf("level %d (%d entities, duration: %d\" %02d')\n",
		current_level, levels.RemainingEntities(), levels.GetDuration() / 60, levels.GetDuration() % 60);

	std::wstring format = I18n::t("menu.story.intro");
	std::string content;
	wstr_to_utf8(content, format);

	content = str_sprintf(content.c_str(), current_level, levels.GetDescription(), levels.RemainingEntities());
	str_replace(content, "\\n", "\n");

	description_.SetText(content);
	// centered on screen
	sf::FloatRect rect = description_.GetRect();
	description_.SetPosition(
		(Game::WIDTH  - rect.GetWidth())  / 2,
		(Game::HEIGHT - rect.GetHeight()) / 2
	);
}
