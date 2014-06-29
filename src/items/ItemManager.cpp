#include <stdexcept>
#include "ItemManager.hpp"
#include "Weapon.hpp"
#include "utils/tinyxml/tinyxml2.h"


ItemManager& ItemManager::GetInstance()
{
	static ItemManager self;
	return self;
}


ItemManager::ItemManager()
{
}


ItemManager::~ItemManager()
{
}


int ItemManager::LoadItems(const std::string& filename)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != 0)
	{
		std::string error = "Cannot load items from " + filename + ": " + doc.GetErrorStr1();
		throw std::runtime_error(error);
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	// weapons
	tinyxml2::XMLElement* elem_class = root->FirstChildElement("weapons")->FirstChildElement("class");
	while (elem_class != NULL)
	{
		tinyxml2::XMLElement* elem_weapon = elem_class->FirstChildElement("weapon");
		WeaponData data;
		data.loadClassFromXml(elem_class);
		while (elem_weapon != NULL)
		{
			if (data.loadFromXml(elem_weapon))
			{
				weapons_.push_back(data);
			}
			elem_weapon = elem_weapon->NextSiblingElement();
		}
		elem_class = elem_class->NextSiblingElement();
	}
	// generic items
	ParseGenericItems(root, "shields", ItemData::SHIELD);
	ParseGenericItems(root, "heatsinks", ItemData::HEATSINK);
	ParseGenericItems(root, "hulls", ItemData::HULL);
	ParseGenericItems(root, "engines", ItemData::ENGINE);
	return items_.size() + weapons_.size();
}


const ItemData* ItemManager::GetItemData(ItemData::Type type, int level) const
{
	const ItemData* data = GetGenericItemData(type, level);
	if (data != NULL)
	{
		return data;
	}

	for (WeaponList::const_iterator it = weapons_.begin(); it != weapons_.end(); ++it)
	{
		data = &(*it);
		if (data->getType() == type && data->getLevel() == level)
		{
			return data;
		}
	}
#ifdef DEBUG
	std::cout << "[ItemManager] item not found: " << ItemData::TypeToString(type) << ", level " << level << std::endl;
#endif
	return NULL;
}


const WeaponData* ItemManager::GetWeaponData(const char* id, int level) const
{
	for (WeaponList::const_iterator it = weapons_.begin(); it != weapons_.end(); ++it)
	{
		if (it->getID() == id && (level == it->getLevel()))
		{
			return &(*it);
		}
	}
	return NULL;
}


const GenericItemData* ItemManager::GetGenericItemData(ItemData::Type type, int level) const
{
	for (GenericItemList::const_iterator it = items_.begin(); it != items_.end(); ++it)
	{
		const GenericItemData* data = &(*it);
		if (data->getType() == type && data->getLevel() == level)
		{
			return data;
		}
	}
	return NULL;
}


void ItemManager::ParseGenericItems(tinyxml2::XMLElement* elem, const char* tagname, ItemData::Type type)
{
	elem = elem->FirstChildElement(tagname);
	if (elem != NULL)
	{
		elem = elem->FirstChildElement();
		while (elem != NULL)
		{
			GenericItemData data(type);
			data.LoadFromXml(elem);
			items_.push_back(data);
			elem = elem->NextSiblingElement();
		}
	}
	else
	{
		std::cerr << "item: tag " << tagname << " not found" << std::endl;
	}
}


