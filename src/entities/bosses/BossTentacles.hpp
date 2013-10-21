#ifndef BOSSTENTACLES_HPP
#define BOSSTENTACLES_HPP

#include "entities/Damageable.hpp"
#include "entities/Animator.hpp"
#include "items/Weapon.hpp"


class BossTentacles: public Damageable
{
public:
	BossTentacles();

	BossTentacles* clone() const;

	// override
	void onUpdate(float frametime);

	// override
	void onInit();

	// override
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
	Entity* target_;
	Animator m_animator;
};

#endif // BOSSTENTACLES_HPP
