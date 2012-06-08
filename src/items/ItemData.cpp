#include "ItemData.hpp"

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
		case ItemData::LASER_CANNON:
			return "item.laser";
		case ItemData::PLASMA_CANNON:
			return "item.plasma";
		case ItemData::ENGINE:
			return "item.engine";
		case ItemData::ARMOR:
			return "item.armor";
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


bool ItemData::LoadFromXml(TiXmlElement* elem)
{
	elem->QueryIntAttribute("price", &price_);
	elem->QueryIntAttribute("level", &level_);
	return true;
}


int ItemData::GetPrice() const
{
	return price_;
}


int ItemData::GetLevel() const
{
	return level_;
}
