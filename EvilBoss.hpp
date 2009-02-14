#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "Ennemy.hpp"
#include "Weapon.hpp"


/**
 * Il est très méchant, et il en veut!
 */
class EvilBoss: public Ennemy
{
public:
	EvilBoss(const sf::Vector2f& offset, Entity* target);
	
	void Move(float frametime);
	void Collide(Entity& ent);
	void Action();
	void Hit(int damage);
	
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
};

#endif /* guard EVILBOSS_HPP */

