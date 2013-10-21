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

	// override
	void onInit();

	void setMovementPattern(MovementPattern movement);

	void setAttackPattern(AttackPattern attack);

	// override
	Spaceship* clone() const;

	void onUpdate(float frametime);

	void onDestroy();

	Weapon<>& getWeapon() { return m_weapon; }

	void setPoints(int points);
	int getPoints() const;

private:
	AttackPattern   m_attack;
	MovementPattern m_movement;

	int      speed_;
	float    base_y_;
	float    base_x_;
	float    angle_;

	int       m_points;
	Weapon<>  m_weapon;
	Entity*   m_target;
	Animator  m_animator;
	sf::Clock m_destroyed_at;
};


#endif // SPACESHIP_HPP
