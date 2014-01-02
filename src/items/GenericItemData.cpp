#include "GenericItemData.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "utils/tinyxml/tinyxml2.h"


GenericItemData::GenericItemData(ItemData::Type type)
{
	SetType(type);
	int_value_ = 0;
}


bool GenericItemData::LoadFromXml(tinyxml2::XMLElement* elem)
{
	ItemData::LoadFromXml(elem);

	const char* attribute = NULL;
	switch (GetType())
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


std::wstring GenericItemData::BuildDescriptionString() const
{
	std::wstring s = _t(std::string(TypeToString()) + "_info");
	const wchar_t* keyword = NULL;
	switch (GetType())
	{
		case ItemData::HEATSINK:
			keyword = L"{heat}";
			break;
		case ItemData::HULL:
			keyword = L"{hp}";
			break;
		case ItemData::ENGINE:
			keyword = L"{speed}";
			break;
		case ItemData::SHIELD:
			keyword = L"{points}";
			break;
		default:
			break;
	}
	if (keyword != NULL)
	{
		wstr_self_replace(s, keyword, std::to_wstring(int_value_));
	}
	return s;
}


int GenericItemData::GetValue() const
{
	return int_value_;
}
