#ifndef ARMORY_HPP
#define ARMORY_HPP

#include "CreditCounterBase.hpp"

class ArmoryMenu: public CreditCounterBase
{
public:
	ArmoryMenu();

private:
	// override
	void EventCallback(int id);
};

#endif // ARMORY_HPP
