#include "ItemData.hpp"
#include "utils/tinyxml/tinyxml2.h"


ItemData::ItemData(Type type):
	m_type(type),
	m_level(0),
	m_price(0)
{
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
	return ItemData::TypeToString(m_type);
}


bool ItemData::LoadFromXml(tinyxml2::XMLElement* elem)
{
	elem->QueryIntAttribute("price", &m_price);
	elem->QueryIntAttribute("level", &m_level);
	return true;
}


ItemData::Type ItemData::getType() const
{
	return m_type;
}


int ItemData::getLevel() const
{
	return m_level;
}


int ItemData::getPrice() const
{
	return m_price;
}
