#include "PlayerSave.hpp"
#include "LevelManager.hpp"
#include "utils/IniParser.hpp"
#include "items/ItemManager.hpp"


int PlayerSave::m_highscore = 0;
int PlayerSave::m_credits = 0;
int PlayerSave::m_items[] = {0};
PlayerSave::Initializer PlayerSave::m_init;

int PlayerSave::getItemLevel(ItemData::Type type)
{
	return m_items[type];
}


void PlayerSave::setItemLevel(ItemData::Type type, int level)
{
	if (level < 1)
		level = 1;

	if (ItemManager::GetInstance().GetItemData(type, level) == NULL)
		level = 1;

	m_items[type] = level;
}


int PlayerSave::getCredits()
{
	return m_credits;
}


void PlayerSave::updateCredits(int diff)
{
	m_credits += diff;
}


int PlayerSave::getHighscore()
{
	return m_highscore;
}


void PlayerSave::setHighscore(int highscore)
{
	m_highscore = highscore;
}


void PlayerSave::loadFromConfig(IniParser& config)
{
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


void PlayerSave::saveToConfig(IniParser& config)
{
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


PlayerSave::Initializer::Initializer()
{
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		m_items[i] = 1;
	}
}
