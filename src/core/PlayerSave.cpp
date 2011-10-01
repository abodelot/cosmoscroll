#include "PlayerSave.hpp"
#include "LevelManager.hpp"


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
	else if (level > 3)
		level = 3;
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


void PlayerSave::LoadFromConfig(ConfigParser& config)
{
	config.SeekSection("Player");
	int level = 1;
	config.ReadItem("last_unlocked_level", level);
	LevelManager::GetInstance().SetLastUnlocked(level);

	level = 1;
	config.ReadItem("current_level", level);
	LevelManager::GetInstance().SetCurrent(level);

	config.ReadItem("credits", available_credits_);
	config.ReadItem("lvl_laser1", items_[ItemData::LASER1]);
	config.ReadItem("lvl_laser2", items_[ItemData::LASER2]);
	config.ReadItem("lvl_shield", items_[ItemData::SHIELD]);
	config.ReadItem("lvl_armor", items_[ItemData::ARMOR]);
	config.ReadItem("lvl_engine", items_[ItemData::ENGINE]);
	config.ReadItem("lvl_heatsink", items_[ItemData::HEATSINK]);
	for (int i = 0; i < ItemData::_COUNT; ++i)
	{
		SetItemLevel((ItemData::Type) i, items_[i]);
	}
}


void PlayerSave::SaveToConfig(ConfigParser& config) const
{
	config.SeekSection("Player");
	config.WriteItem("current_level", LevelManager::GetInstance().GetCurrent());
	config.WriteItem("last_unlocked_level", LevelManager::GetInstance().GetLastUnlocked());
	config.WriteItem("credits", available_credits_);
	config.WriteItem("lvl_laser1", items_[ItemData::LASER1]);
	config.WriteItem("lvl_laser2", items_[ItemData::LASER2]);
	config.WriteItem("lvl_shield", items_[ItemData::SHIELD]);
	config.WriteItem("lvl_armor", items_[ItemData::ARMOR]);
	config.WriteItem("lvl_engine", items_[ItemData::ENGINE]);
	config.WriteItem("lvl_heatsink", items_[ItemData::HEATSINK]);
}

