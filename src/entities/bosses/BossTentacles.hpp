#ifndef BOSSTENTACLES_HPP
#define BOSSTENTACLES_HPP

#include "entities/Damageable.hpp"
#include "entities/Animator.hpp"
#include "items/Weapon.hpp"


class BossTentacles: public Damageable
{
public:
	BossTentacles();

	// callbacks ---------------------------------------------------------------

	void onInit();

	void onUpdate(float frametime);

	void onDestroy();

private:
	enum Move
	{
		INIT,
		LURK,
		IDLE,
		CHARGE
	};

	Move move_;

	float timer_;
	int speed_x_;
	int speed_y_;
	Weapon<> m_weapon;
	Entity*  m_target;
	Animator m_animator;
};

#endif // BOSSTENTACLES_HPP
