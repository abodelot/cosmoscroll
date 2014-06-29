#include "Item.hpp"
#include "utils/tinyxml/tinyxml2.h"


Item::Item(Type type):
	m_type(type),
	m_level(0),
	m_price(0)
{
}


const char* Item::TypeToString(Type type)
{
	switch (type)
	{
		case Item::WEAPON:   return "item.laser";
		case Item::ENGINE:   return "item.engine";
		case Item::HULL:     return "item.hull";
		case Item::SHIELD:   return "item.shield";
		case Item::HEATSINK: return "item.heatsink";
		default: break;
	}
	return NULL;
}


const char* Item::TypeToString() const
{
	return Item::TypeToString(m_type);
}


bool Item::LoadFromXml(tinyxml2::XMLElement* elem)
{
	elem->QueryIntAttribute("price", &m_price);
	elem->QueryIntAttribute("level", &m_level);
	return true;
}


Item::Type Item::getType() const
{
	return m_type;
}


int Item::getLevel() const
{
	return m_level;
}


int Item::getPrice() const
{
	return m_price;
}
