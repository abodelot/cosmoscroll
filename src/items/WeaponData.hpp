#ifndef WEAPONDATA_HPP
#define WEAPONDATA_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "ItemData.hpp"
#include "entities/Projectile.hpp"

template <class T>
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

	template <class T>
	void InitWeapon(Weapon<T>& weapon) const;

private:
	std::string id_;         // weapon name
	float heat_cost_;        // cost per shot
	float fire_rate_;        // shot per second
	int speed_;              // bullet speed (pixels per second)
	int damage_;             // inflicted damage
	const sf::Texture* image_; // hit image
	const sf::SoundBuffer* sound_; // sfx
};

template <class T>
void WeaponData::InitWeapon(Weapon<T>& weapon) const
{
	weapon.setTexture(image_);
	weapon.setFireRate(fire_rate_);
	weapon.setHeatCost(heat_cost_);
	weapon.setDamage(damage_);
	weapon.setVelociy(speed_);
	weapon.setSound(sound_);
}

#endif // WEAPONDATA_HPP
