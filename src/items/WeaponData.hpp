#ifndef WEAPONDATA_HPP
#define WEAPONDATA_HPP

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "ItemData.hpp"
#include "entities/Projectile.hpp"


class Weapon;

class WeaponData: public ItemData
{
public:
	WeaponData();

	bool loadClassFromXml(tinyxml2::XMLElement* elem);
	bool loadFromXml(tinyxml2::XMLElement* elem);

	std::wstring getDescription() const override;

	const std::string& getID() const;

	void InitWeapon(Weapon& weapon) const;

private:
	std::string id_;         // weapon name
	float heat_cost_;        // cost per shot
	float fire_rate_;        // shot per second
	int speed_;              // bullet speed (pixels per second)
	int damage_;             // inflicted damage
	const sf::Texture* m_texture; // hit image
	const sf::SoundBuffer* sound_; // sfx
};

#endif // WEAPONDATA_HPP
