#include "AboutMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"

#define COSMOSCROLL_ABOUT \
		L"Version : 0.2-devel\n\n" \
		"Auteurs :\n" \
		"     Alexandre Bodelot\n" \
		"     Arnaud Wolff\n\n" \
		"Licence : GPL"


AboutMenu::AboutMenu()
{
	SetTitle(L"À propos de CosmoScroll");

	info_text_.SetText(COSMOSCROLL_ABOUT);
	info_text_.SetPosition(Game::MARGIN_X, 120);
	info_text_.SetColor(sf::Color::White);
	info_text_.SetFont(GET_FONT());

	logos_.SetImage(GET_IMG("gui/libs-logo"));
	logos_.SetY(Game::HEIGHT - logos_.GetSize().y);
	logos_.SetX((Game::WIDTH - logos_.GetSize().x) / 2);
	SetOffset(Game::MARGIN_X, 340);
	AddOption("Retour", 1);
}


void AboutMenu::Callback(int id)
{
	if (id == 1)
	{
		Game::GetInstance().SetNextScene(Game::SC_MainMenu);
	}
}


void AboutMenu::Show(sf::RenderTarget& target) const
{
	BaseMenu::Show(target);
	target.Draw(info_text_);
	target.Draw(logos_);
}

