#ifndef PLAYERSAVE_HPP
#define PLAYERSAVE_HPP

#include "items/ItemData.hpp"
#include "utils/ConfigParser.hpp"

class PlayerSave
{
public:
	PlayerSave();

	int AvailableCredits() const;
	int LevelOf(ItemData::Type type) const;

	void SetItemLevel(ItemData::Type type, int level);


	void LoadFromConfig(ConfigParser& config);
	void SaveToConfig(ConfigParser& config) const;

	int GetCredits() const;
	void UpdateCredits(int diff);

private:
	int available_credits_;
	int items_[ItemData::_COUNT];
};

#endif // PLAYERSAVE_HPP
