#ifndef ITEMMANAGER_HPP
#define ITEMMANAGER_HPP

#include <string>
#include <map>


#include "WeaponData.hpp"
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
	const WeaponData& GetWeaponData(const char* id) const;

private:
	ItemManager();
	~ItemManager();

	/**
	 * Parse a weapon entry from an XML element
	 */
	void ParseWeapon(TiXmlElement* elem);

	typedef std::map<std::string, WeaponData> WeaponMap;
	WeaponMap weapon_defs_;

};

#endif // ITEMMANAGER_HPP
