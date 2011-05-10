#include "AboutMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"


#define COSMOSCROLL_LICENCE "GPL"
#define COSMOSCROLL_AUTHORS "    Alexandre Bodelot\n" \
							"    Arnaud Wolff\n\n"


AboutMenu::AboutMenu()
{
	SetTitle(_t("menu.about.title"));

	std::string str = std::string(_t("menu.about.version")) + " " + COSMOSCROLL_VERSION + "\n\n";
	str += std::string(_t("menu.about.authors")) + " \n" + COSMOSCROLL_AUTHORS;
	str += std::string(_t("menu.about.licence")) + " " + COSMOSCROLL_LICENCE;

	info_text_.SetText(str);
	info_text_.SetPosition(120, 100);
	info_text_.SetFont(GET_FONT());

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

