#include "AboutMenu.hpp"
#include "../core/Game.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/I18n.hpp"

#define COSMOSCROLL_VERSION L"0.3-devel"
#define COSMOSCROLL_LICENCE L"GPL"
#define COSMOSCROLL_AUTHORS L"\n" \
							 "    Alexandre Bodelot\n" \
							 "    Arnaud Wolff\n\n"


AboutMenu::AboutMenu()
{
	SetTitle(I18n::t("menu.about.title"));

	std::wstring str = I18n::t("menu.about.version") + L" " + COSMOSCROLL_VERSION + L"\n\n";
	str += I18n::t("menu.about.authors") + L" " + COSMOSCROLL_AUTHORS;
	str += I18n::t("menu.about.licence") + L" " + COSMOSCROLL_LICENCE;

	info_text_.SetText(str);
	info_text_.SetPosition(120, 120);
	info_text_.SetFont(GET_FONT());

	logos_.SetImage(GET_IMG("gui/libs-logo"));
	logos_.SetY(Game::HEIGHT - logos_.GetSize().y);
	logos_.SetX((Game::WIDTH - logos_.GetSize().x) / 2);

	(new CosmoButton(this, I18n::t("menu.back"), 210, 340))->SetCallbackID(1);
}


void AboutMenu::EventCallback(int id)
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

