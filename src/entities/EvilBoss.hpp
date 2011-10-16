#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "Entity.hpp"
#include "items/Weapon.hpp"


/**
 * Il est très méchant, et il en veut !
 */
class EvilBoss: public Entity
{
public:
	EvilBoss(const sf::Vector2f& position);

	EvilBoss* Clone() const;

	// override
	void Update(float frametime);

	// override
	void TakeDamage(int damage);

	// override
	void SetTarget(Entity* target);

	// override
	void OnDestroy();

private:
	enum Phase
	{
		EVIL = 600, MORE_EVIL = 400, DAMN_EVIL = 200
	};

	Phase phase_, next_;
	int speed_x_, speed_y_;
	Weapon eye_left_;
	Weapon eye_right_;
	Weapon canon_;
	Entity* target_;
};

#endif // EVILBOSS_HPP
