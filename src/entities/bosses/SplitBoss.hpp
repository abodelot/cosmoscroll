#ifndef SPLITBOSS_HPP
#define SPLITBOSS_HPP

#include "entities/Damageable.hpp"
#include "items/Weapon.hpp"


class SplitBoss: public Damageable
{
public:
	SplitBoss(bool split = true);

	void onUpdate(float frametime) override;

	void takeDamage(int damage) override;

	void onInit() override;

	void onDestroy() override;

private :
	int speed_x_,speed_y_;
	Entity* target_;
	bool split_mode_;
	Weapon<> eye_left_;
	Weapon<> eye_right_;
};


#endif // SPLITBOSS_HPP
