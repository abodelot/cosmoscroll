#ifndef PLAYERSAVE_HPP
#define PLAYERSAVE_HPP

#include "items/ItemData.hpp"

class IniParser;

class PlayerSave
{
public:
	PlayerSave();

	int AvailableCredits() const;
	int LevelOf(ItemData::Type type) const;

	void SetItemLevel(ItemData::Type type, int level);


	void LoadFromConfig(IniParser& config);
	void SaveToConfig(IniParser& config) const;

	int GetCredits() const;
	void UpdateCredits(int diff);

private:
	int available_credits_;
	int items_[ItemData::_COUNT];
};

#endif // PLAYERSAVE_HPP
