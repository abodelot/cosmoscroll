#ifndef ARMORY_HPP
#define ARMORY_HPP

#include "scenes/BaseMenu.hpp"
#include "CreditCounterWidget.hpp"
#include "ShipItemWidget.hpp"


class ArmoryMenu: public BaseMenu
{
public:
	ArmoryMenu();

	void OnFocus() override;

private:
	void EventCallback(int id) override;

	ShipItemWidget*      m_items[Item::_COUNT];
	CreditCounterWidget* m_credits;
};

#endif // ARMORY_HPP
