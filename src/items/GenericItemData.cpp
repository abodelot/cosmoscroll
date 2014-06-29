#include "GenericItemData.hpp"
#include "utils/I18n.hpp"
#include "utils/StringUtils.hpp"
#include "utils/tinyxml/tinyxml2.h"


GenericItemData::GenericItemData(Item::Type type):
	Item(type),
	m_value(0)
{
}


bool GenericItemData::LoadFromXml(tinyxml2::XMLElement* elem)
{
	Item::LoadFromXml(elem);

	const char* attribute = NULL;
	switch (getType())
	{
		case Item::HULL:
			attribute = "hp";
			break;
		case Item::ENGINE:
			attribute = "speed";
			break;
		case Item::SHIELD:
			attribute = "points";
			break;
		case Item::HEATSINK:
			attribute = "heat";
			break;
		default:
			break;
	}
	if (attribute != NULL)
	{
		elem->QueryIntAttribute(attribute, &m_value);
	}
	return true;
}


std::wstring GenericItemData::getDescription() const
{
	const char* template_str = "";
	const char* keyword = "";
	switch (getType())
	{
		case Item::HEATSINK:
			template_str = "item.heatsink_info";
			keyword      = "{heat}";
			break;
		case Item::HULL:
			template_str = "item.hull_info";
			keyword      = "{hp}";
			break;
		case Item::ENGINE:
			template_str = "item.engine_info";
			keyword      = "{speed}";
			break;
		case Item::SHIELD:
			template_str = "item.shield_info";
			keyword      = "{points}";
			break;
		default:
			break;
	}
	return I18n::templatize(template_str, keyword, m_value);
}


int GenericItemData::GetValue() const
{
	return m_value;
}
