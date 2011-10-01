#include "ItemData.hpp"

ItemData::ItemData()
{
	price_ = 0;
	level_ = 0;
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
