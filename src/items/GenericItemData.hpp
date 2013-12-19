#ifndef GENERICITEMDATA_HPP
#define GENERICITEMDATA_HPP

#include "ItemData.hpp"

/**
 * Any item with an extra int field
 */
class GenericItemData: public ItemData
{
public:
	GenericItemData(ItemData::Type type);

	bool LoadFromXml(tinyxml2::XMLElement* elem) override;

	std::wstring BuildDescriptionString() const override;

	int GetValue() const;

private:
	int int_value_;
};

#endif // GENERICITEMDATA_HPP
