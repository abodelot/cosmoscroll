#ifndef CANON_HPP
#define CANON_HPP

#include "MultiPartEntity.hpp"
#include "items/Weapon.hpp"

class Canon: public MultiPartEntity
{
public:
	Canon();

	Canon* clone() const;

	void onInit();
	void onUpdate(float frametime);

private:
	Weapon<> weapon_;
};

#endif // CANON_HPP
