#include "EndGameMenu.hpp"
#include "../core/Game.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"


EndGameMenu::EndGameMenu()
{
	SetTitle(I18n::t("menu.gameover.title"));
	result_.SetColor(sf::Color::White);
	result_.SetFont(GET_FONT());
	result_.SetSize(30);
	result_.SetPosition(120, 150);

	SetOffsetY(300);

	AddOption(I18n::t("menu.gameover.play_again"), 0);
	AddOption(I18n::t("menu.back_main_menu"), 1);
}


void EndGameMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.Draw(result_);
}


void EndGameMenu::Poke()
{
	EntityManager& entities = EntityManager::GetInstance();

	int min = (int) entities.GetTimer() / 60;
	int sec = (int) entities.GetTimer() % 60;
	std::wstring text;
	if (entities.GetTimer() > entities.GetArcadeRecord())
	{
		entities.UpdateArcadeRecord();
		text = str_sprintf(I18n::t("menu.gameover.record").c_str(), min, sec);
	}
	else
	{
		text = str_sprintf(I18n::t("menu.gameover.no_record").c_str(), min, sec);
	}
	result_.SetText(text);
}


void EndGameMenu::Callback(int id)
{
	switch (id)
	{
		case 0:
			EntityManager::GetInstance().InitMode(EntityManager::MODE_ARCADE);
			Game::GetInstance().SetNextScene(Game::SC_InGameScene);
			break;
		case 1:
			Game::GetInstance().SetNextScene(Game::SC_MainMenu);
			break;
	}
}
