#include "AboutMenu.hpp"
#include "core/Game.hpp"
#include "core/Constants.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"

#include <sstream>


AboutMenu::AboutMenu()
{
    setTitle(_t("about.title"));

    std::ostringstream oss;
    oss << VERSION_STRING << "\n"
        << _t("about.contact") << " " << APP_AUTHOR << "\n"
        << _t("about.website") << " " << COSMOSCROLL_WEBSITE << "\n"
        << _t("about.licence") << " " << APP_LICENSE << "\n\n"
        << "Build: " << __DATE__;

    gui::Label* about_text = new gui::Label(this, oss.str());
    about_text->setFont(Resources::getFont("Vera.ttf"));
    about_text->setPosition(85, 110);
    about_text->setCharacterSize(12);

    new gui::Image(this, Resources::getTexture("gui/libraries-logo.png"), 70, 280);

    gui::Button* b = new CosmoButton(this, _t("back"));
    b->setPosition(210, 410);
    b->SetCallbackID(1);
}


void AboutMenu::EventCallback(int id)
{
    if (id == 1)
    {
        Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
    }
}

