#include "UserSettings.hpp"
#include "LevelManager.hpp"
#include "utils/IniParser.hpp"
#include "utils/I18n.hpp"
#include "items/ItemManager.hpp"

bool UserSettings::panel_on_top = true;

int UserSettings::m_highscore = 0;
int UserSettings::m_credits = 0;
int UserSettings::m_items[] = {0};
UserSettings::Initializer UserSettings::m_init;

int UserSettings::getItemLevel(ItemData::Type type)
{
	return m_items[type];
}


void UserSettings::setItemLevel(ItemData::Type type, int level)
{
	if (level < 1)
		level = 1;

	if (ItemManager::GetInstance().GetItemData(type, level) == NULL)
		level = 1;

	m_items[type] = level;
}


int UserSettings::getCredits()
{
	return m_credits;
}


void UserSettings::updateCredits(int diff)
{
	m_credits += diff;
}


int UserSettings::getHighscore()
{
	return m_highscore;
}


void UserSettings::setHighscore(int highscore)
{
	m_highscore = highscore;
}


void UserSettings::loadFromConfig(IniParser& config)
{
	config.SeekSection("Settings");
	// language
	std::string lang = config.Get("language");
	if (lang.empty() || !I18n::getInstance().loadFromCode(lang))
	{
		I18n::getInstance().loadSystemLanguage();
	}

	// Panel position
	config.Get("panel_on_top", panel_on_top);

	config.SeekSection("Player");
	int level = 1;
	config.Get("last_unlocked_level", level);
	LevelManager::getInstance().setLastUnlocked(level);

	level = 1;
	config.Get("current_level", level);
	LevelManager::getInstance().setCurrent(level);

	config.Get("credits",          m_credits);
	config.Get("arcade_highscore", m_highscore);

	config.Get("lvl_laser",    m_items[ItemData::WEAPON]);
	config.Get("lvl_shield",   m_items[ItemData::SHIELD]);
	config.Get("lvl_hull",     m_items[ItemData::HULL]);
	config.Get("lvl_engine",   m_items[ItemData::ENGINE]);
	config.Get("lvl_heatsink", m_items[ItemData::HEATSINK]);
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		setItemLevel((ItemData::Type) i, m_items[i]);
	}
}


void UserSettings::saveToConfig(IniParser& config)
{
	config.SeekSection("Settings");
	config.Set("panel_on_top", panel_on_top);
	config.Set("language", I18n::getInstance().getLangCode());

	config.SeekSection("Player");
	config.Set("current_level", LevelManager::getInstance().getCurrent());
	config.Set("last_unlocked_level", LevelManager::getInstance().getLastUnlocked());
	config.Set("credits", m_credits);
	config.Set("arcade_highscore", m_highscore);
	config.Set("lvl_laser",    m_items[ItemData::WEAPON]);
	config.Set("lvl_shield",   m_items[ItemData::SHIELD]);
	config.Set("lvl_hull",     m_items[ItemData::HULL]);
	config.Set("lvl_engine",   m_items[ItemData::ENGINE]);
	config.Set("lvl_heatsink", m_items[ItemData::HEATSINK]);
}


UserSettings::Initializer::Initializer()
{
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		m_items[i] = 1;
	}
}
