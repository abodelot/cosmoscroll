#include "PlayerSave.hpp"


PlayerSave::PlayerSave()
{
	available_credits_ = 0;
	last_unlocked_level_ = 0;
	current_level_ = 0;
	for (int i = 0; i < UpgradeItem::_UP_COUNT; ++i)
	{
		items_[i] = 0;
	}
}


int PlayerSave::LevelOf(UpgradeItem::Type type) const
{
	return items_[type];
}


void PlayerSave::LoadFromConfig(ConfigParser& config)
{
	config.SeekSection("Player");
	config.ReadItem("current_level", current_level_);
	config.ReadItem("last_unlocked_level", last_unlocked_level_);
	config.ReadItem("credits", available_credits_);
	config.ReadItem("lvl_laser1", items_[UpgradeItem::UP_LASER1]);
	config.ReadItem("lvl_laser2", items_[UpgradeItem::UP_LASER2]);
	config.ReadItem("lvl_shield", items_[UpgradeItem::UP_SHIELD]);
	config.ReadItem("lvl_armor", items_[UpgradeItem::UP_ARMOR]);
	config.ReadItem("lvl_engine", items_[UpgradeItem::UP_ENGINE]);
	config.ReadItem("lvl_heatsink", items_[UpgradeItem::UP_HEATSINK]);
}


void PlayerSave::SaveToConfig(ConfigParser& config) const
{
	config.SeekSection("Player");
	config.WriteItem("current_level", current_level_);
	config.WriteItem("last_unlocked_level", last_unlocked_level_);
	config.WriteItem("credits", available_credits_);
	config.WriteItem("lvl_laser1", items_[UpgradeItem::UP_LASER1]);
	config.WriteItem("lvl_laser2", items_[UpgradeItem::UP_LASER2]);
	config.WriteItem("lvl_shield", items_[UpgradeItem::UP_SHIELD]);
	config.WriteItem("lvl_armor", items_[UpgradeItem::UP_ARMOR]);
	config.WriteItem("lvl_engine", items_[UpgradeItem::UP_ENGINE]);
	config.WriteItem("lvl_heatsink", items_[UpgradeItem::UP_HEATSINK]);
}

