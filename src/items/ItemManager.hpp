#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <string>
#include <list>
#include <iostream>

#include "WeaponData.hpp"
#include "GenericItemData.hpp"

class ItemManager
{
public:

	static ItemManager& GetInstance();

	/**
	 * Load items definitions from XML
	 * @param filename: XML document
	 */
	int LoadItems(const std::string& filename);

	/**
	 * Initialize weapon options
	 * @param id: weapon XML id
	 * @param weapon: weapon to initialize
	 */
	template <class T>
	void InitWeapon(Weapon<T>& weapon, const char* id, int level) const;

	/**
	 * Get item data from any item type
	 */
	const ItemData* GetItemData(ItemData::Type, int level) const;

	/**
	 * Get item data from a weapon
	 */
	const WeaponData* GetWeaponData(const char* id, int level=1) const;

	/**
	 * Get item data from a generic item
	 */
	const GenericItemData* GetGenericItemData(ItemData::Type, int level) const;

private:
	ItemManager();
	~ItemManager();


	void ParseGenericItems(tinyxml2::XMLElement* elem, const char* tagname, ItemData::Type type);

	typedef std::list<WeaponData> WeaponList;
	WeaponList weapons_;

	typedef std::list<GenericItemData> GenericItemList;
	GenericItemList items_;
};

template <class T>
void ItemManager::InitWeapon(Weapon<T>& weapon, const char* id, int level) const
{
	const WeaponData* data = GetWeaponData(id, level);
	if (data == NULL)
	{
		std::cerr << "can't initialize weapon " << id << " (unknown id)" << std::endl;
	}
	else
	{
		data->InitWeapon(weapon);
	}
}


#endif // ITEMMANAGER_HPP
