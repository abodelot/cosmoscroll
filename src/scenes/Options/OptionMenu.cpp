#include "OptionMenu.hpp"
#include "core/Game.hpp"
#include "utils/I18n.hpp"


OptionMenu::OptionMenu()
{
    setTitle(_t("options.title"));

    gui::VBoxLayout layout(210, 120);

    layout.Add(new CosmoButton(this, _t("options.keyboard")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_KeyboardMenu);
    });
    layout.Add(new CosmoButton(this, _t("options.joystick")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_JoystickMenu);
    });
    layout.Add(new CosmoButton(this, _t("options.audio")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_AudioMenu);
    });
    layout.Add(new CosmoButton(this, _t("options.settings")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_SettingsMenu);
    });
    layout.Add(new CosmoButton(this, _t("back")))->setCallback([]() {
        Game::getInstance().setCurrentScreen(Game::SC_MainMenu);
    });
}
