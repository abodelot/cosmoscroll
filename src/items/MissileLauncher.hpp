#ifndef MISSILELAUNCHER_HPP
#define MISSILELAUNCHER_HPP

#include "Weapon.hpp"

class MissileLauncher: public Weapon
{
protected:
	// inherited
	void ThrowHit(const sf::Vector2f& offset, float angle);
};

#endif // MISSILELAUNCHER_HPP
