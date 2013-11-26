#ifndef PLAYERSAVE_HPP
#define PLAYERSAVE_HPP

#include "items/ItemData.hpp"

class IniParser;

class PlayerSave
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

private:
	static int m_highscore;
	static int m_credits;
	static int m_items[ItemData::_COUNT];
};

#endif // PLAYERSAVE_HPP
