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

	void onInit() override;
	void onUpdate(float frametime) override;
	void onDestroy() override;

private:
	enum State { INIT, LURK, IDLE, CHARGE };

	State        m_state;
	float        m_timer;
	sf::Vector2f m_speed;
	Weapon<>     m_weapon;
	Entity*      m_target;
	Animator     m_animator;
};

#endif // TENTACULATBOSS_HPP
