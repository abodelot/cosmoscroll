#ifndef GUN_TOWER_HPP
#define GUN_TOWER_HPP

#include "ComplexEntity.hpp"
#include "items/Weapon.hpp"

class GunTower: public ComplexEntity
{
public:
	GunTower(const sf::Vector2f& position);

	void onInit();

	void onUpdate(float frametime);

	GunTower* clone() const;

private:
	Weapon<> weapon_;
	Entity* target_;
};

#endif // GUN_TOWER_HPP
