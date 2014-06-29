#ifndef BRAINBOSS_HPP
#define BRAINBOSS_HPP

#include "entities/MultiPartEntity.hpp"
#include "entities/Animator.hpp"
#include "entities/Weapon.hpp"

class BrainBoss: public MultiPartEntity
{
public:
	BrainBoss();

	// callbacks ---------------------------------------------------------------

	void onUpdate(float frametime) override;
	void onPartDestroyed(const Part& part) override;
	void onPartDamaged(const Part& part) override;

private:
	enum State { WAIT, WAIT_ATTACK, MOVE };

	State      m_state;
	float      m_state_timer;
	Animator   m_eye_animator;
	Weapon     m_weapon;
	float      m_weapon_angle;
	float      m_xspeed;
};

#endif // BRAINBOSS_HPP
