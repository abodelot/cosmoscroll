#include "ItemManager.hpp"
#include "Weapon.hpp"
#include "tinyxml/tinyxml.h"
#include "utils/Error.hpp"


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
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		Error::Log << "Cannot load items definitions:\n" << filename << "\n" << doc.ErrorDesc() << "\n";
		throw Error::Exception();
	}

	TiXmlElement* root = doc.RootElement();
	// weapons
	TiXmlElement* elem = root->FirstChildElement("weapons")->FirstChildElement();
	while (elem != NULL)
	{
		WeaponData data;
		if (data.LoadFromXml(elem))
		{
			weapons_.push_back(data);
		}
		elem = elem->NextSiblingElement();
	}
	// generic items
	ParseGenericItems(root, "shields", ItemData::SHIELD);
	ParseGenericItems(root, "heatsinks", ItemData::HEATSINK);
	ParseGenericItems(root, "armors", ItemData::ARMOR);
	ParseGenericItems(root, "engines", ItemData::ENGINE);
	return items_.size() + weapons_.size();
}


void ItemManager::InitWeapon(const char* id, Weapon* weapon) const
{
	const WeaponData* data = GetWeaponData(id);
	if (data == NULL)
	{
		std::cerr << "can't initialize weapon " << id << " (unknown id)" << std::endl;
	}
	else
	{
		data->InitWeapon(weapon);
	}
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
		if (data->GetType() == type && data->GetLevel() == level)
		{
			return data;
		}
	}
#ifdef DEBUG
	std::cout << "[ItemManager] item not found: " << ItemData::TypeToString(type)
		<< " (level " << level << ")" << std::endl;
#endif
	return NULL;
}


const WeaponData* ItemManager::GetWeaponData(const char* id, int level) const
{
	for (WeaponList::const_iterator it = weapons_.begin(); it != weapons_.end(); ++it)
	{
		if (it->GetID() == id && (level == 0 || level == it->GetLevel()))
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
		if (data->GetType() == type && data->GetLevel() == level)
		{
			return data;
		}
	}
	return NULL;
}


void ItemManager::ParseGenericItems(TiXmlElement* elem, const char* tagname, ItemData::Type type)
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


