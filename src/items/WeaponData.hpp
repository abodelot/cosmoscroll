#ifndef WEAPONDATA_HPP
#define WEAPONDATA_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "ItemData.hpp"

class Weapon;

class WeaponData: public ItemData
{
public:
	WeaponData();

	// override
	bool LoadFromXml(TiXmlElement* elem);

	// override
	std::wstring BuildDescriptionString() const;

	const std::string& GetID() const;
	void InitWeapon(Weapon* weapon) const;

private:
	std::string id_;         // weapon name
	float heat_cost_;        // cost per shot
	float fire_rate_;        // shot per second
	int speed_;              // bullet speed (pixels per second)
	int damage_;             // inflicted damage
	const sf::Texture* image_; // hit image
	const sf::SoundBuffer* sound_; // sfx
};

#endif // WEAPONDATA_HPP
