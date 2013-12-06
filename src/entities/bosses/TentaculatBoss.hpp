#ifndef TENTACULATBOSS_HPP
#define TENTACULATBOSS_HPP

#include "entities/Damageable.hpp"
#include "entities/Animator.hpp"
#include "items/Weapon.hpp"

class TentaculatBoss: public Damageable
{
public:
	TentaculatBoss();

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

	Move         m_move;
	float        m_timer;
	sf::Vector2f m_speed;
	Weapon<>     m_weapon;
	Entity*      m_target;
	Animator     m_animator;
};

#endif // TENTACULATBOSS_HPP
