#include "AboutMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "utils/Resources.hpp"
#include "utils/StringUtils.hpp"
#include "utils/I18n.hpp"



AboutMenu::AboutMenu()
{
	SetTitle(_t("menu.about.title"));

	std::wostringstream oss;
	oss << _t("menu.about.version") << L" " << GAME_VERSION << L"\n\n"
	    << _t("menu.about.authors") << L" \n" << GAME_AUTHORS << L"\n\n"
	    << _t("menu.about.licence") << L" " << GAME_LICENSE;

	info_text_.SetText(oss.str());
	info_text_.SetPosition(120, 100);
	info_text_.SetFont(GetMenuFont());

	logos_.SetImage(Resources::GetImage("gui/libs-logo.png"));
	logos_.SetPosition(430, 110);


	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->SetPosition(210, 340);
	b->SetCallbackID(1);
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

