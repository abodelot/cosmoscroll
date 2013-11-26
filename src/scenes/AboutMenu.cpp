#include "AboutMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"



AboutMenu::AboutMenu()
{
	SetTitle(_t("menu.about.title"));

	std::wostringstream oss;
	oss << _t("menu.about.version") << L" " << GAME_VERSION << L"\n\n"
	    << _t("menu.about.authors") << L" \n" << GAME_AUTHORS << L"\n\n"
	    << _t("menu.about.licence") << L" " << GAME_LICENSE;

	gui::Label* about_text = new gui::Label(this, oss.str());
	about_text->setPosition(120, 100);
	about_text->setCharacterSize(30);

	new gui::Image(this, Resources::getTexture("gui/libs-logo.png"), 430, 110);

	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->setPosition(210, 340);
	b->SetCallbackID(1);
}


void AboutMenu::EventCallback(int id)
{
	if (id == 1)
	{
		Game::getInstance().setNextScene(Game::SC_MainMenu);
	}
}

