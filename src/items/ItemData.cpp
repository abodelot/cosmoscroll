#include "ItemData.hpp"
#include "utils/tinyxml/tinyxml2.h"

ItemData::ItemData()
{
	price_ = 0;
	level_ = 0;
	type_ = _UNSET;
}


const char* ItemData::TypeToString(Type type)
{
	switch (type)
	{
		case ItemData::WEAPON:
			return "item.laser";
		case ItemData::ENGINE:
			return "item.engine";
		case ItemData::HULL:
			return "item.hull";
		case ItemData::SHIELD:
			return "item.shield";
		case ItemData::HEATSINK:
			return "item.heatsink";
		default:
			break;
	}
	return NULL;
}


const char* ItemData::TypeToString() const
{
	return ItemData::TypeToString(type_);
}


bool ItemData::LoadFromXml(tinyxml2::XMLElement* elem)
{
	elem->QueryIntAttribute("price", &price_);
	elem->QueryIntAttribute("level", &level_);
	return true;
}


int ItemData::getPrice() const
{
	return price_;
}


int ItemData::getLevel() const
{
	return level_;
}
