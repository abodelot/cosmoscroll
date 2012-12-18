#ifndef CANON_HPP
#define CANON_HPP

#include "ComplexEntity.hpp"
#include "items/Weapon.hpp"

class Canon: public ComplexEntity
{
public:
	Canon(const sf::Vector2f& position);

	void Update(float frametime);

	Canon* Clone() const;

private:
	Weapon<> weapon_;
};

#endif // CANON_HPP
