#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <string>
#include <list>

#include "WeaponData.hpp"
#include "GenericItemData.hpp"

class Weapon;

class ItemManager
{
public:

	static ItemManager& GetInstance();

	/**
	 * Load items definitions from XML
	 * @param filename: XML document
	 */
	bool LoadItems(const char* filename);

	/**
	 * Initialize weapon options
	 * @param id: weapon XML id
	 * @param weapon: weapon to initialize
	 */
	void InitWeapon(const char* id, Weapon* weapon) const;

	const WeaponData* GetWeaponData(const char* id) const;

	const ItemData* GetItemData(ItemData::Type, int level) const;

private:
	ItemManager();
	~ItemManager();


	void ParseGenericItems(TiXmlElement* elem, const char* tagname, ItemData::Type type);

	typedef std::list<WeaponData> WeaponList;
	WeaponList weapons_;

	typedef std::list<GenericItemData> GenericItemList;
	GenericItemList items_;
};

#endif // ITEMMANAGER_HPP
