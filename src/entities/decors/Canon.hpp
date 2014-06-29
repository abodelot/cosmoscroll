#ifndef CANON_HPP
#define CANON_HPP

#include "entities/MultiPartEntity.hpp"
#include "entities/Weapon.hpp"

class Canon: public MultiPartEntity
{
public:
	Canon();

	void onInit();
	void onUpdate(float frametime);

private:
	Weapon m_weapon;
};

#endif // CANON_HPP
