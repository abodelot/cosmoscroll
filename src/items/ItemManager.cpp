#include "ItemManager.hpp"
#include "Weapon.hpp"

#include "tinyxml/tinyxml.h"
#include "utils/MediaManager.hpp"


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


bool ItemManager::LoadItems(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		std::cerr << "Can't load item definitions: " << filename << ", " << doc.ErrorDesc() << std::endl;
		return false;
	}

	TiXmlElement* root = doc.RootElement();

	// weapons
	puts("* loading weapons...");
	TiXmlElement* elem = root->FirstChildElement("weapons")->FirstChildElement();

	while (elem != NULL)
	{
		const char* p = elem->Attribute("id");
		if (p != NULL)
		{
			weapon_defs_[p].LoadFromXml(elem);
		}
		else
		{
			std::cerr << "weapon id is missing (ignored)" << std::endl;
		}
		elem = elem->NextSiblingElement();
	}
}


void ItemManager::InitWeapon(const char* id, Weapon* weapon) const
{
	const WeaponData& data = GetWeaponData(id);
	data.InitWeapon(weapon);
}


const WeaponData& ItemManager::GetWeaponData(const char* id) const
{
	WeaponMap::const_iterator it = weapon_defs_.find(id);
	if (it == weapon_defs_.end())
	{
		std::cerr << "can't initialize weapon " << id << " (unknown id)" << std::endl;
		abort();
	}
	return it->second;
}



