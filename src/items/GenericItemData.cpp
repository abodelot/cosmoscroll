#include "GenericItemData.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"


GenericItemData::GenericItemData(ItemData::Type type)
{
	SetType(type);
	int_value_ = 0;
}


bool GenericItemData::LoadFromXml(TiXmlElement* elem)
{
	ItemData::LoadFromXml(elem);

	const char* attribute = NULL;
	switch (GetType())
	{
		case ItemData::ARMOR:
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


std::wstring GenericItemData::BuildDescriptionString(bool include_price) const
{
	std::wstring s = _t(std::string(TypeToString()) + "_info");
	if (include_price)
	{
		s = s + L"\n" + _t("item.price");
	}

	const wchar_t* keyword = NULL;
	switch (GetType())
	{
		case ItemData::HEATSINK:
			keyword = L"{heat}";
			break;
		case ItemData::ARMOR:
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
		wstr_self_replace(s, keyword, to_wstring(int_value_));
		wstr_self_replace(s, L"{price}", to_wstring(GetPrice()));
	}
	return s;
}


int GenericItemData::GetValue() const
{
	return int_value_;
}
