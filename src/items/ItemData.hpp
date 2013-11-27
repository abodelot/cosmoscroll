#ifndef ITEMDATA_HPP
#define ITEMDATA_HPP

#include <string>
#include "utils/tinyxml/tinyxml2.h"


class ItemData
{
public:
	enum Type
	{
		ARMOR,
		ENGINE,
		SHIELD,
		HEATSINK,
		WEAPON,
		_COUNT,
		_UNSET
	};

	ItemData();

	static const char* TypeToString(Type type);

	const char* TypeToString() const;

	virtual bool LoadFromXml(tinyxml2::XMLElement* elem);

	int getPrice() const;
	int getLevel() const;

	inline Type GetType() const { return type_; }
	void SetType(Type type) { type_ = type; }

	virtual std::wstring BuildDescriptionString() const = 0;


private:
	int price_;
	int level_;
	Type type_;
};

#endif // ITEMDATA_HPP
