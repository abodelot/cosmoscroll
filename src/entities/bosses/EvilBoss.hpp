#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "entities/Damageable.hpp"
#include "items/Weapon.hpp"


/**
 * Il est très méchant, et il en veut !
 */
class EvilBoss: public Damageable
{
public:
	EvilBoss();

	void takeDamage(int damage) override;

	// callbacks ---------------------------------------------------------------

	void onInit() override;
	void onUpdate(float frametime) override;
	void onDestroy() override;

private:
	enum State
	{
		EVIL = 600, MORE_EVIL = 400, DAMN_EVIL = 200
	};

	State        m_state;
	State        m_next_state;
	sf::Vector2f m_speed;
	Weapon<>     m_eye_left;
	Weapon<>     m_eye_right;
	Weapon<>     m_mouth;
	Entity*      m_target;
};

#endif // EVILBOSS_HPP
