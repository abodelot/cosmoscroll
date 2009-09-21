#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Entity.hpp"
#include "Weapon.hpp"
#include "../core/Animated.hpp"

class SpaceShip: public Entity, public Animated
{
public:
	SpaceShip(const char* animation, int hp, int speed);
	~SpaceShip();

	/**
	 * Allouer une copie identique du vaisseau
	 */
	SpaceShip* Clone() const;

	// inherited
	void SetTarget(Entity* target);

	/**
	 * Obtenir l'arme au vaisseau
	 */
	Weapon* GetWeapon();

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int damage);

private:
	int speed_;
	Weapon weapon_;
	Entity* target_;
};


#endif // SPACESHIP_HPP
