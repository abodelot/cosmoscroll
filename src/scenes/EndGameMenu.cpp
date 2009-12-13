#include "EndGameMenu.hpp"
#include "../core/Game.hpp"
#include "../entities/EntityManager.hpp"
#include "../utils/StringUtils.hpp"


EndGameMenu::EndGameMenu()
{
	SetTitle(L"Game Over");
	result_.SetColor(sf::Color::White);
	result_.SetFont(GET_FONT());
	result_.SetSize(30);
	result_.SetPosition(Game::MARGIN_X, 150);

	SetOffset(Game::MARGIN_X, 300);

	AddOption("Rejouer", 0);
	AddOption("Revenir au menu principal", 1);
}


void EndGameMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.Draw(result_);
}


void EndGameMenu::Poke()
{
	EntityManager& entities = EntityManager::GetInstance();

	std::string text;
	int min = (int) entities.GetTimer() / 60;
	int sec = (int) entities.GetTimer() % 60;
	if (entities.GetTimer() > entities.GetArcadeRecord())
	{
		entities.UpdateArcadeRecord();
		text = str_sprintf("Nouveau record !\n    %02d min %02d sec !", min, sec);
	}
	else
	{
		text = str_sprintf("Vous avez tenu seulement\n%02d min %02d sec ...", min, sec);
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
