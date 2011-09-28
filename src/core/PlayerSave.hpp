#ifndef PLAYERSAVE_HPP
#define PLAYERSAVE_HPP

#include "scenes/story/UpgradeItem.hpp"
#include "utils/ConfigParser.hpp"

class PlayerSave
{
public:
	PlayerSave();

	int AvailableCredits() const;
	int CurrentLevel() const;
	int LastUnlockedLevel() const;
	int LevelOf(UpgradeItem::Type type) const;

	void LoadFromConfig(ConfigParser& config);
	void SaveToConfig(ConfigParser& config) const;

private:
	int available_credits_;
	int current_level_;
	int last_unlocked_level_;
	int items_[UpgradeItem::_UP_COUNT];
};

#endif // PLAYERSAVE_HPP
