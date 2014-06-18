#include "AboutMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"


AboutMenu::AboutMenu()
{
	SetTitle(_t("menu.about.title"));

	std::wostringstream oss;
	oss << VERSION_STRING << "\n"
	    << _t("menu.about.contact") << " " << APP_AUTHOR << "\n"
	    << _t("menu.about.website") << " " << COSMOSCROLL_WEBSITE << "\n"
	    << _t("menu.about.licence") << " " << APP_LICENSE << "\n\n"
	    << "Build: " << __DATE__;

	gui::Label* about_text = new gui::Label(this, oss.str());
	about_text->setFont(Resources::getFont("Vera.ttf"));
	about_text->setPosition(85, 110);
	about_text->setCharacterSize(12);

	new gui::Image(this, Resources::getTexture("gui/libraries-logo.png"), 70, 280);

	gui::Button* b = new CosmoButton(this, _t("menu.back"));
	b->setPosition(210, 410);
	b->SetCallbackID(1);
}


void AboutMenu::EventCallback(int id)
{
	if (id == 1)
	{
		Game::getInstance().setNextScene(Game::SC_MainMenu);
	}
}

