#ifndef CANON_HPP
#define CANON_HPP

#include "ComplexEntity.hpp"
#include "items/Weapon.hpp"

class Canon: public ComplexEntity
{
public:
	Canon(const sf::Vector2f& position);

	void onUpdate(float frametime);

	Canon* clone() const;

private:
	Weapon<> weapon_;
};

#endif // CANON_HPP
