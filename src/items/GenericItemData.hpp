#ifndef GENERICITEMDATA_HPP
#define GENERICITEMDATA_HPP

#include "Item.hpp"

/**
 * Any item with an extra int field
 */
class GenericItemData: public Item
{
public:
	GenericItemData(Item::Type type);

	bool LoadFromXml(tinyxml2::XMLElement* elem) override;

	std::wstring getDescription() const override;

	int GetValue() const;

private:
	int m_value;
};

#endif // GENERICITEMDATA_HPP
