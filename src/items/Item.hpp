#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

namespace tinyxml2
{
class XMLElement;
}

class Item
{
public:
	enum Type
	{
		HULL,
		ENGINE,
		SHIELD,
		HEATSINK,
		WEAPON,
		_COUNT,
	};

	Item(Type type);

	static const char* TypeToString(Type type);

	const char* TypeToString() const;

	virtual bool LoadFromXml(tinyxml2::XMLElement* elem);

	Type getType() const;

	int getLevel() const;

	int getPrice() const;

	virtual std::wstring getDescription() const = 0;

private:
	Type m_type;
	int m_level;
	int m_price;
};

#endif // ITEM_HPP
