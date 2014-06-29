#include "GenericItemData.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "utils/tinyxml/tinyxml2.h"


GenericItemData::GenericItemData(ItemData::Type type):
	ItemData(type),
	int_value_(0)
{
}


bool GenericItemData::LoadFromXml(tinyxml2::XMLElement* elem)
{
	ItemData::LoadFromXml(elem);

	const char* attribute = NULL;
	switch (getType())
	{
		case ItemData::HULL:
			attribute = "hp";
			break;
		case ItemData::ENGINE:
			attribute = "speed";
			break;
		case ItemData::SHIELD:
			attribute = "points";
			break;
		case ItemData::HEATSINK:
			attribute = "heat";
			break;
		default:
			break;
	}
	if (attribute != NULL)
	{
		elem->QueryIntAttribute(attribute, &int_value_);
	}
	return true;
}


std::wstring GenericItemData::getDescription() const
{
	const char* template_str = "";
	const char* keyword = "";
	switch (getType())
	{
		case ItemData::HEATSINK:
			template_str = "item.heatsink_info";
			keyword      = "{heat}";
			break;
		case ItemData::HULL:
			template_str = "item.hull_info";
			keyword      = "{hp}";
			break;
		case ItemData::ENGINE:
			template_str = "item.engine_info";
			keyword      = "{speed}";
			break;
		case ItemData::SHIELD:
			template_str = "item.shield_info";
			keyword      = "{points}";
			break;
		default:
			break;
	}
	return I18n::templatize(template_str, keyword, int_value_);
}


int GenericItemData::GetValue() const
{
	return int_value_;
}
