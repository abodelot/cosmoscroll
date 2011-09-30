#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Entity.hpp"
#include "items/Weapon.hpp"
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
	// Suivre le joueur
	void move_magnet(float frametime);
	// Avancer tout droit
	void move_straight(float frametime);
	// courbe sinus
	void move_sinus(float frametime);

	// ATTACK PATTERN
	// - Viser automatiquement le joueur
	void attack_auto_aim();
	// - Tirer si le joueur passe devant
	void attack_on_sight();
	// - ne pas attaquer le joueur
	void attack_none();

	void (SpaceShip::*move_pattern_)(float);
	void (SpaceShip::*attack_pattern_)();

	int speed_;
	int base_y_;
	Weapon weapon_;
	Entity* target_;
};


#endif // SPACESHIP_HPP
