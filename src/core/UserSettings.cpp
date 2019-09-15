#include "UserSettings.hpp"
#include "core/Services.hpp"
#include "Input.hpp"
#include "SoundSystem.hpp"
#include "core/Factory.hpp"
#include "utils/I18n.hpp"
#include "utils/IniParser.hpp"

bool UserSettings::show_hud = true;

int UserSettings::s_credits = 0;
int UserSettings::s_items[] = {0};
UserSettings::Init UserSettings::s_ctor;

UserSettings::Init::Init()
{
    for (int i = 0; i < Item::_COUNT; ++i) {
        s_items[i] = 1;
    }
}


int UserSettings::getItemLevel(Item::Type type)
{
    return s_items[type];
}


void UserSettings::setItemLevel(Item::Type type, int level)
{
    // Reset level if item does not exist
    if (!Services::getFactory().hasItem(type, level)) {
        level = 1;
    }

    s_items[type] = level;
}


int UserSettings::getCredits()
{
    return s_credits;
}


void UserSettings::updateCredits(int diff)
{
    s_credits += diff;
}


void UserSettings::loadFromConfig(IniParser& config)
{
    config.seekSection("Settings");
    // language
    std::string lang = config.get("language");
    if (lang.empty() || !g_i18n.loadFromCode(lang)) {
        g_i18n.loadFromLocale();
    }

    // Panel position
    show_hud = config.get("show_hud", show_hud);

    config.seekSection("Player");

    int level = config.get("last_unlocked_level", 1);
    LevelParser& levelParser = Services::getLevelParser();
    levelParser.setLastUnlocked(level);

    level = config.get("current_level", 1);
    levelParser.setCurrent(level);

    s_credits = config.get("credits", 0);

    s_items[Item::WEAPON] =   config.get("lvl_laser", 1);
    s_items[Item::SHIELD] =   config.get("lvl_shield", 1);
    s_items[Item::HULL] =     config.get("lvl_hull", 1);
    s_items[Item::ENGINE] =   config.get("lvl_engine", 1);
    s_items[Item::HEATSINK] = config.get("lvl_heatsink", 1);
    for (int i = 0; i < Item::_COUNT; ++i) {
        setItemLevel((Item::Type) i, s_items[i]);
    }

    config.seekSection("Keyboard");
    Input::setKeyBinding(config.get("move_up",    Input::getKeyBinding(Action::UP)), Action::UP);
    Input::setKeyBinding(config.get("move_down",  Input::getKeyBinding(Action::DOWN)), Action::DOWN);
    Input::setKeyBinding(config.get("move_left",  Input::getKeyBinding(Action::LEFT)), Action::LEFT);
    Input::setKeyBinding(config.get("move_right", Input::getKeyBinding(Action::RIGHT)), Action::RIGHT);
    Input::setKeyBinding(config.get("laser",      Input::getKeyBinding(Action::USE_LASER)), Action::USE_LASER);
    Input::setKeyBinding(config.get("cooler",     Input::getKeyBinding(Action::USE_COOLER)), Action::USE_COOLER);
    Input::setKeyBinding(config.get("missile",    Input::getKeyBinding(Action::USE_MISSILE)), Action::USE_MISSILE);

    config.seekSection("Joystick");
    Input::setButtonBinding(config.get("pause",   Input::getButtonBinding(Action::PAUSE)), Action::PAUSE);
    Input::setButtonBinding(config.get("laser",   Input::getButtonBinding(Action::USE_LASER)), Action::USE_LASER);
    Input::setButtonBinding(config.get("cooler",  Input::getButtonBinding(Action::USE_COOLER)), Action::USE_COOLER);
    Input::setButtonBinding(config.get("missile", Input::getButtonBinding(Action::USE_MISSILE)), Action::USE_MISSILE);

    config.seekSection("Audio");
    SoundSystem& soundSystem = Services::getSoundSystem();
    soundSystem.enableMusic(config.get("enable_music", true));
    soundSystem.setMusicVolume(config.get("music_volume", 100));
    soundSystem.enableSound(config.get("enable_sound", true));
    soundSystem.setSoundVolume(config.get("sound_volume", 100));
}


void UserSettings::saveToConfig(IniParser& config)
{
    config.seekSection("Settings");
    config.set("show_hud", show_hud);
    config.set("language", g_i18n.getCurrentLanguage());

    config.seekSection("Player");
    LevelParser& levelParser = Services::getLevelParser();
    config.set("current_level",       levelParser.getCurrent());
    config.set("last_unlocked_level", levelParser.getLastUnlocked());
    config.set("credits",             s_credits);
    config.set("lvl_laser",    s_items[Item::WEAPON]);
    config.set("lvl_shield",   s_items[Item::SHIELD]);
    config.set("lvl_hull",     s_items[Item::HULL]);
    config.set("lvl_engine",   s_items[Item::ENGINE]);
    config.set("lvl_heatsink", s_items[Item::HEATSINK]);

    config.seekSection("Keyboard");
    config.set("move_up",    Input::getKeyBinding(Action::UP));
    config.set("move_down",  Input::getKeyBinding(Action::DOWN));
    config.set("move_left",  Input::getKeyBinding(Action::LEFT));
    config.set("move_right", Input::getKeyBinding(Action::RIGHT));
    config.set("laser",      Input::getKeyBinding(Action::USE_LASER));
    config.set("cooler",     Input::getKeyBinding(Action::USE_COOLER));
    config.set("missile",    Input::getKeyBinding(Action::USE_MISSILE));

    config.seekSection("Joystick");
    config.set("pause",       Input::getButtonBinding(Action::PAUSE));
    config.set("laser",       Input::getButtonBinding(Action::USE_LASER));
    config.set("cooler",      Input::getButtonBinding(Action::USE_COOLER));
    config.set("missile",     Input::getButtonBinding(Action::USE_MISSILE));

    config.seekSection("Audio");
    SoundSystem& soundSystem = Services::getSoundSystem();
    config.set("enable_music", soundSystem.isMusicEnabled());
    config.set("music_volume", soundSystem.getMusicVolume());
    config.set("enable_sound", soundSystem.isSoundEnabled());
    config.set("sound_volume", soundSystem.getSoundVolume());
}
