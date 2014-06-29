#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <string>
#include <list>

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
	void InitWeapon(Weapon& weapon, const char* id, int level) const;

	/**
	 * Get item data from any item type
	 */
	const Item* GetItemData(Item::Type, int level) const;

	/**
	 * Get item data from a generic item
	 */
	const GenericItemData* GetGenericItemData(Item::Type, int level) const;

private:
	ItemManager();
	~ItemManager();

	/**
	 * Get item data from a weapon
	 */
	const WeaponData* GetWeaponData(const char* id, int level=1) const;

	void ParseGenericItems(tinyxml2::XMLElement* elem, const char* tagname, Item::Type type);

	typedef std::list<WeaponData> WeaponList;
	WeaponList weapons_;

	typedef std::list<GenericItemData> GenericItemList;
	GenericItemList items_;
};

#endif // ITEMMANAGER_HPP
