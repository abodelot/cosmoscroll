#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Entity.hpp"
#include "items/Weapon.hpp"
#include "Animator.hpp"

class SpaceShip: public Entity
{
public:
	enum MovementPattern
	{
		MAGNET, LINE, SINUS, CIRCLE
	};

	enum AttackPattern
	{
		AUTO_AIM, ON_SIGHT, NONE
	};

	SpaceShip(const Animation& animation, int hp, int speed);
	~SpaceShip();

	// override
	void onInit();

	void setMovementPattern(MovementPattern movement);

	void setAttackPattern(AttackPattern attack);

	// override
	SpaceShip* Clone() const;

	// override
	void SetTarget(Entity* target);

	// override
	void Update(float frametime);

	// override
	void OnDestroy();

	Weapon<>& getWeapon() { return weapon_; }

private:
	AttackPattern   attack_;
	MovementPattern movement_;

	int      speed_;
	float    base_y_;
	float    base_x_;
	float    angle_;

	Weapon<> weapon_;
	Entity*  target_;
	Animator animator_;
};


#endif // SPACESHIP_HPP
