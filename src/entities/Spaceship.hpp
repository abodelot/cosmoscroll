#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Entity.hpp"
#include "items/Weapon.hpp"
#include "Animator.hpp"

class Spaceship: public Entity
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

	Spaceship(const Animation& animation, int hp, int speed);
	~Spaceship();

	// override
	void onInit();

	void setMovementPattern(MovementPattern movement);

	void setAttackPattern(AttackPattern attack);

	// override
	Spaceship* clone() const;

	void collide(Entity& entity) const { entity.onCollision(*this); }

	// override
	void onUpdate(float frametime);

	// override
	void onDestroy();

	Weapon<>& getWeapon() { return m_weapon; }

private:
	AttackPattern   m_attack;
	MovementPattern m_movement;

	int      speed_;
	float    base_y_;
	float    base_x_;
	float    angle_;

	Weapon<> m_weapon;
	Entity*  m_target;
	Animator m_animator;
};


#endif // SPACESHIP_HPP
