#ifndef ARMORY_HPP
#define ARMORY_HPP

#include "CreditCounterBase.hpp"
#include "ShipPartWidget.hpp"


class ArmoryMenu: public CreditCounterBase
{
public:
	ArmoryMenu();

	void OnFocus();

private:
	// override
	void EventCallback(int id);

	void ShowDialog(bool visible);

	bool BuyItem();

	void LoadItem(ItemData::Type type);

	struct Dialog
	{
		static const int WIDTH = 360;
		static const int HEIGHT = 180;
		int x, y;
		gui::Image* background;
		gui::Label* lab_item;
		gui::Label* current_level;
		gui::Label* current_level_details;
		gui::Label* next_level;
		gui::Label* next_level_details;
		gui::Label* price;
		gui::Button* but_back;
		gui::Button* but_buy;
	} dialog_;

	ShipPartWidget* items_[ItemData::_COUNT];
	gui::Label* lab_info_;
	ItemData::Type current_type_;
};

#endif // ARMORY_HPP
