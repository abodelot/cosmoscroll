#ifndef GUN_TOWER_HPP
#define GUN_TOWER_HPP

#include "entities/MultiPartEntity.hpp"
#include "items/Weapon.hpp"

class GunTower: public MultiPartEntity
{
public:
	GunTower();

	void onInit();

	void onUpdate(float frametime);

private:
	Weapon<> m_weapon;
	Entity*  m_target;
};

#endif // GUN_TOWER_HPP
