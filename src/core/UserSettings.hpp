#ifndef USERSETTINGS_HPP
#define USERSETTINGS_HPP

#include "items/Item.hpp"

class IniParser;

class UserSettings {
public:
    static void loadFromConfig(IniParser& config);
    static void saveToConfig(IniParser& config);

    static int getItemLevel(Item::Type type);
    static void setItemLevel(Item::Type type, int level);

    static int getCredits();
    static void updateCredits(int diff);

    // Settings
    static bool show_hud;

private:
    static int s_credits;
    static int s_items[Item::_COUNT];

    static struct Init { Init(); } s_ctor;
};

#endif // USERSETTINGS_HPP
