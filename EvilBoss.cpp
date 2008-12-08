#include "EvilBoss.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"
#include "Game.hpp"

#include <iostream>

#define SPEED 25
#define CHILDS 8

#define L_EYE_OFFSET        sf::Vector2f(131,  67)
#define R_EYE_OFFSET	    sf::Vector2f(233, 70)

#define L_MOUTH_X_OFFSET 85
#define R_MOUTH_X_OFFSET 201
#define MOUTH_Y_OFFSET 128


EvilBoss::EvilBoss(const sf::Vector2f& offset, Entity* target) :
	Ennemy(offset, GET_IMG("evil_boss"), 500, target),
	eye_left_(Weapon::PLASMACANNON, this),
	eye_right_(Weapon::PLASMACANNON, this),
	canon_(Weapon::DEVILSEYES, this),
	game_(Game::GetInstance())
{
	left_ = true;
	phase_ = EVIL;
}


void EvilBoss::Move(float frametime)
{
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
	eye_left_.Update(frametime);
	eye_right_.Update(frametime);
	canon_.Update(frametime);
}


void EvilBoss::Collide(Entity& ent)
{
	(void) ent;
}


void EvilBoss::Hit(int damage)
{
	Entity::Hit(damage);
	if (phase_ == EVIL && hp_ < 200)
	{
		phase_ = MORE_EVIL;
		sprite_.SetImage(GET_IMG("evil_boss2"));
		canon_.SetTriple(true);
	}
}


void EvilBoss::Action()
{
	float radians_L = ANGLE(target_->GetPosition(), sprite_.GetPosition() + L_EYE_OFFSET);
	float radians_R = ANGLE(target_->GetPosition(), sprite_.GetPosition() + R_EYE_OFFSET);

	eye_left_.Shoot(sprite_.GetPosition() + L_EYE_OFFSET, radians_L);
	eye_right_.Shoot(sprite_.GetPosition() + R_EYE_OFFSET, radians_R);
	sf::Vector2f randV2f, my = sprite_.GetPosition();
	randV2f.x = my.x + sf::Randomizer::Random(L_MOUTH_X_OFFSET, R_MOUTH_X_OFFSET);
	randV2f.y = my.y + MOUTH_Y_OFFSET;
	canon_.Shoot(randV2f, ANGLE(target_->GetPosition(), randV2f));
}

