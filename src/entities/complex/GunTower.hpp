#ifndef GUN_TOWER_HPP
#define GUN_TOWER_HPP

#include "ComplexEntity.hpp"
#include "items/Weapon.hpp"

class GunTower: public ComplexEntity
{
public:
	GunTower(const sf::Vector2f& position);

	void Update(float frametime);

	GunTower* Clone() const;

	// for auto-aim on player
	virtual void SetTarget(Entity* target);
private:
	Weapon weapon_;
	Entity* target_;
};

#endif // GUN_TOWER_HPP
