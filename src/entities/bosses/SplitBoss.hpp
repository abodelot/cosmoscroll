#ifndef SPLITBOSS_HPP
#define SPLITBOSS_HPP

#include "entities/Entity.hpp"
#include "items/Weapon.hpp"


class SplitBoss: public Entity
{
public:
	SplitBoss(sf::Vector2f& pos,int HP = 200,bool split = true);

	SplitBoss* clone() const;

	// override
	void onUpdate(float frametime);

	// override
	void takeDamage(int damage);

	// override
	void onInit();

	// override
	void onDestroy();

private :
	int speed_x_,speed_y_;
	Entity* target_;
	bool split_mode_;
	Weapon<> eye_left_;
	Weapon<> eye_right_;
};


#endif // SPLITBOSS_HPP
