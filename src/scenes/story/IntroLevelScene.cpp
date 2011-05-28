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
	description_.SetFont(GetMenuFont());
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

	std::wstring intro = _t("menu.story.intro");
	wstr_self_replace(intro, L"{level}", to_wstring(current_level));
	wstr_self_replace(intro, L"{description}", I18n::DecodeUTF8(levels.GetDescription()));
	wstr_self_replace(intro, L"{count}", to_wstring(levels.RemainingEntities()));
	wstr_self_replace(intro, L"\\n", L"\n");
	description_.SetText(intro);

	// centered on screen
	sf::FloatRect rect = description_.GetRect();
	description_.SetPosition(
		(Game::WIDTH  - rect.GetWidth())  / 2,
		(Game::HEIGHT - rect.GetHeight()) / 2
	);
}
