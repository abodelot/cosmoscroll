#ifndef ARMORY_HPP
#define ARMORY_HPP

#include "CreditCounterBase.hpp"
#include "UpgradeItem.hpp"


class ArmoryMenu: public CreditCounterBase
{
public:
	ArmoryMenu();

private:
	// override
	void EventCallback(int id);

	void ShowDialog(bool visible);

	void LoadItem(UpgradeItem::Type type);

	struct Dialog
	{
		gui::Image* background;
		gui::Label* lab_item;
		gui::Label* current_level;
		gui::Label* current_level_details;
		gui::Label* next_level;
		gui::Label* next_level_details;
		gui::Button* but_back;
		gui::Button* but_buy;
	} dialog_;

	struct ItemData
	{
		int price;
	};

	struct ArmorData: public ItemData
	{
		int armor_points;
	};

	struct EngineData: public ItemData
	{
		float speed;
	};

	struct ShieldData: public ItemData
	{
		int shield_points;
	};

	struct HeatSinkData: public ItemData
	{
		int max_heat;
	};

	struct LaserData: public ItemData
	{
		int damage;
		float speed;
	};

	//ItemData* item_data_[UpgradeItem::_UP_COUNT];

};

#endif // ARMORY_HPP
