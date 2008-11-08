#include "EvilBoss.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"
#include "Game.hpp"

#include <iostream>

#define SPEED 25
#define CHILDS 8

#define L_EYE_OFFSET        sf::Vector2f(176,  90)
#define R_EYE_OFFSET	    sf::Vector2f(316, 92)

#define GUN_OFFSET	    sf::Vector2f(248, 91)

EvilBoss::EvilBoss(const sf::Vector2f& offset, Entity* target) :
    Ennemy(offset, GET_IMG("evil_boss"), 25, target),
	weaponA_(Weapon::HELLFIRE, this),
	weaponB_(Weapon::LASERBEAM, this),
	game_(Game::GetInstance())
{
}


void EvilBoss::Move(float frametime)
{
	frametime_ = frametime;
	std::cerr << "Move()\n";
	bool left = true;
	static bool direction_ = false;
	static float mover_ = 10.0;
	float moving_ = SPEED * frametime;
	mover_ -= moving_;
	if (mover_ < 1 && mover_ > -1)
	{
		mover_ = 30.0;
		direction_ ^= 1;
	}
	if (direction_)
	{
		sprite_.Move(0, -SPEED * frametime);
	}
	else
	{
		sprite_.Move(0, SPEED * frametime);
	}

	if (sprite_.GetPosition().x < target_->GetPosition().x)
    {
    	left = false;
    }

	if (left != left_)
	{
	    sprite_.FlipX(!left);
	    left_ = left;
	}
}

void EvilBoss::Collide(Entity& ent)
{
	(void) ent;
}

void EvilBoss::Action()
{
	static float cpt = 5.0;
	cpt -= frametime_;
	int mode = sf::Randomizer::Random(0, 1);
	if (cpt <= 0)
	{
		cpt = 5.0;
		mode = 2;
	}
	
    std::cerr << "Action() " << mode << "\n";
	float radians_L = ANGLE(target_->GetPosition(), sprite_.GetPosition() + L_EYE_OFFSET);
	float radians_R = ANGLE(target_->GetPosition(), sprite_.GetPosition() + R_EYE_OFFSET);
	//if (mode == 1 || mode == 2)
	//{
		std::cerr << "\tMode A\n";
		weaponA_.Update(frametime_);
		if (weaponA_.Shoot(sprite_.GetPosition() + L_EYE_OFFSET, radians_L) != 0.f)
		{
			weaponA_.Update(10000.f);
			weaponA_.Shoot(sprite_.GetPosition() + R_EYE_OFFSET, radians_R);
		};

	//}
	//if (mode == 2)
	//{
		std::cerr << "\tMode B\n";
		for (int i = 0; i < CHILDS; ++i)
		{
			weaponB_.Shoot(sprite_.GetPosition() + GUN_OFFSET, sf::Randomizer::Random(0, 360));
			weaponB_.Update(1000.0f);
		}
	//}*/
}
