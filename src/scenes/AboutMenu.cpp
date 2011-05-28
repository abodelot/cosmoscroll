#include "AboutMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"


#define COSMOSCROLL_LICENCE L"GPL"
#define COSMOSCROLL_AUTHORS L"    Alexandre Bodelot\n" \
							 "    Arnaud Wolff\n\n"


AboutMenu::AboutMenu()
{
	SetTitle(_t("menu.about.title"));

	std::wostringstream oss;
	oss << _t("menu.about.version") << L" " << COSMOSCROLL_VERSION << L"\n\n"
	    << _t("menu.about.authors") << L" \n" << COSMOSCROLL_AUTHORS
	    << _t("menu.about.licence") << L" " << COSMOSCROLL_LICENCE;

	info_text_.SetText(oss.str());
	info_text_.SetPosition(120, 100);
	info_text_.SetFont(GetMenuFont());

	logos_.SetImage(GET_IMG("gui/libs-logo"));
	logos_.SetY(Game::HEIGHT - logos_.GetSize().y);
	logos_.SetX((Game::WIDTH - logos_.GetSize().x) / 2);

	(new CosmoButton(this, _t("menu.back"), 210, 340))->SetCallbackID(1);
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

