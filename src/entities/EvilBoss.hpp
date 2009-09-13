#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "Entity.hpp"
#include "Weapon.hpp"


/**
 * Il est très méchant, et il en veut !
 */
class EvilBoss: public Entity
{
public:
	EvilBoss(const sf::Vector2f& offset, Entity* target);

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int damage);

private:
	enum Phase
	{
		EVIL, MORE_EVIL
	};

	Phase phase_;
	bool left_;
	Weapon eye_left_;
	Weapon eye_right_;
	Weapon canon_;
	Entity* target_;
};

#endif // EVILBOSS_HPP
