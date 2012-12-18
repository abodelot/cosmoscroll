#ifndef SPLITBOSS_HPP
#define SPLITBOSS_HPP

#include "entities/Entity.hpp"
#include "items/Weapon.hpp"


class SplitBoss: public Entity
{
public:
	SplitBoss( sf::Vector2f& pos,int HP = 300,bool split = true);

	SplitBoss* Clone() const;

	// override
	void Update(float frametime);

	// override
	void TakeDamage(int damage);

	// override
	void SetTarget(Entity* target);

	// override
	void OnDestroy();

private :
	int speed_x_,speed_y_;
	Entity* target_;
	bool split_mode_;
	Weapon<> eye_left_;
	Weapon<> eye_right_;
};


#endif // SPLITBOSS_HPP
