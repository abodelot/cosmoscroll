#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Entity.hpp"
#include "Weapon.hpp"
#include "Animated.hpp"

class SpaceShip: public Entity, public Animated
{
public:
	SpaceShip(const char* animation, int hp, int speed);
	~SpaceShip();

	void SetMovePattern(const char* pattern);

	void SetAttackPattern(const char* pattern);

	/**
	 * Obtenir l'arme au vaisseau
	 */
	Weapon* GetWeapon();

	// inherited
	SpaceShip* Clone() const;

	// inherited
	void SetTarget(Entity* target);

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int damage);

private:
	// MOVEMENT PATTERN
	// - Suivre le joueur
	void MP_MAGNET(float frametime);
	// - Avancer tout droit
	void MP_STRAIGHT(float frametime);

	// ATTACK PATTERN
	// - Viser automatiquement le joueur
	void AP_AUTO_AIM();
	// - Tirer si le joueur passe devant
	void AP_ON_SIGHT();
	// - ne pas attaquer le joueur
	void AP_NO_ATTACK();

	void (SpaceShip::*move_pattern_)(float);
	void (SpaceShip::*attack_pattern_)();

	int speed_;
	Weapon weapon_;
	Entity* target_;
};


#endif // SPACESHIP_HPP
