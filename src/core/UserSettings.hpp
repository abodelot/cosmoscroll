#ifndef USERSETTINGS_HPP
#define USERSETTINGS_HPP

#include "items/ItemData.hpp"

class IniParser;

class UserSettings
{
public:
	static void loadFromConfig(IniParser& config);
	static void saveToConfig(IniParser& config);

	static int getItemLevel(ItemData::Type type);
	static void setItemLevel(ItemData::Type type, int level);

	static int getCredits();
	static void updateCredits(int diff);

	static int getHighscore();
	static void setHighscore(int highscore);

	// Settings
	static bool panel_on_top;

private:
	static int m_highscore;
	static int m_credits;
	static int m_items[ItemData::_COUNT];

	static struct Initializer
	{
		Initializer();
	} m_init;
};

#endif // USERSETTINGS_HPP
