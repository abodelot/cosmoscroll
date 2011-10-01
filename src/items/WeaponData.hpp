#ifndef WEAPONDATA_HPP
#define WEAPONDATA_HPP

#include <SFML/Graphics/Image.hpp>
#include "ItemData.hpp"

class Weapon;

class WeaponData: public ItemData
{
public:
	WeaponData();

	bool LoadFromXml(TiXmlElement* elem);

	void InitWeapon(Weapon* weapon) const;

private:
	float heat_cost_;        // cost per shot
	float fire_rate_;        // shot per second
	int speed_;              // bullet speed (pixels per second)
	int damage_;             // inflicted damage
	const sf::Image* image_; // hit image
	std::string sound_;      // sound name
};

#endif // WEAPONDATA_HPP
