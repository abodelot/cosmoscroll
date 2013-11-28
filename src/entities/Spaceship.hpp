#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Damageable.hpp"
#include "items/Weapon.hpp"
#include "Animator.hpp"

class Spaceship: public Damageable
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

	void setMovementPattern(MovementPattern movement);

	void setAttackPattern(AttackPattern attack);

	Spaceship* clone() const;

	Weapon<>& getWeapon() { return m_weapon; }

	void setPoints(int points);
	int getPoints() const;

	// callbacks ---------------------------------------------------------------

	void onInit();
	void onUpdate(float frametime);
	void onDestroy();

private:
	AttackPattern   m_attack;
	MovementPattern m_movement;

	int      speed_;
	float    base_y_;
	float    angle_;

	int       m_points;
	Weapon<>  m_weapon;
	Entity*   m_target;
	Animator  m_animator;
};


#endif // SPACESHIP_HPP
