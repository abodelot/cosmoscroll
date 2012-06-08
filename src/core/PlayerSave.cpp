#include "PlayerSave.hpp"
#include "LevelManager.hpp"
#include "utils/IniParser.hpp"
#include "items/ItemManager.hpp"


PlayerSave::PlayerSave()
{
	available_credits_ = 0;
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		items_[i] = 1;
	}
}


int PlayerSave::LevelOf(ItemData::Type type) const
{
	return items_[type];
}



void PlayerSave::SetItemLevel(ItemData::Type type, int level)
{
	if (level < 1)
		level = 1;

	if (ItemManager::GetInstance().GetItemData(type, level) == NULL)
		level = 1;

	items_[type] = level;
}


int PlayerSave::GetCredits() const
{
	return available_credits_;
}


void PlayerSave::UpdateCredits(int diff)
{
	available_credits_ += diff;
}


void PlayerSave::LoadFromConfig(IniParser& config)
{
	config.SeekSection("Player");
	int level = 1;
	config.Get("last_unlocked_level", level);
	LevelManager::GetInstance().SetLastUnlocked(level);

	level = 1;
	config.Get("current_level", level);
	LevelManager::GetInstance().SetCurrent(level);

	config.Get("credits",      available_credits_);
	config.Get("lvl_laser",    items_[ItemData::LASER_CANNON]);
	config.Get("lvl_plasma",   items_[ItemData::PLASMA_CANNON]);
	config.Get("lvl_shield",   items_[ItemData::SHIELD]);
	config.Get("lvl_armor",    items_[ItemData::ARMOR]);
	config.Get("lvl_engine",   items_[ItemData::ENGINE]);
	config.Get("lvl_heatsink", items_[ItemData::HEATSINK]);
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		SetItemLevel((ItemData::Type) i, items_[i]);
	}
}


void PlayerSave::SaveToConfig(IniParser& config) const
{
	config.SeekSection("Player");
	config.Set("current_level", LevelManager::GetInstance().GetCurrent());
	config.Set("last_unlocked_level", LevelManager::GetInstance().GetLastUnlocked());
	config.Set("credits", available_credits_);
	config.Set("lvl_laser", items_[ItemData::LASER_CANNON]);
	config.Set("lvl_plasma", items_[ItemData::PLASMA_CANNON]);
	config.Set("lvl_shield", items_[ItemData::SHIELD]);
	config.Set("lvl_armor", items_[ItemData::ARMOR]);
	config.Set("lvl_engine", items_[ItemData::ENGINE]);
	config.Set("lvl_heatsink", items_[ItemData::HEATSINK]);
}

