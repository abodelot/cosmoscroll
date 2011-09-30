#ifndef PLAYERSAVE_HPP
#define PLAYERSAVE_HPP

#include "scenes/story/UpgradeItem.hpp"
#include "utils/ConfigParser.hpp"

class PlayerSave
{
public:
	PlayerSave();

	int AvailableCredits() const;
	int LevelOf(UpgradeItem::Type type) const;
	void SetItemLevel(UpgradeItem::Type type, int level);


	void LoadFromConfig(ConfigParser& config);
	void SaveToConfig(ConfigParser& config) const;

	int GetCredits() const;
	void UpdateCredits(int diff);

private:
	int available_credits_;
	int items_[UpgradeItem::_UP_COUNT];
};

#endif // PLAYERSAVE_HPP
