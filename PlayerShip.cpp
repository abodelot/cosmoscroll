#include "PlayerShip.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#ifdef DEBUG
#include <iostream>
#endif

#include "Math.hpp"
#include "ParticleSystem.hpp"
#include "Settings.hpp"

#define GUN_OFFSET				  sf::Vector2f(52, 24)

#define DEFAULT_SPEED			200

#define COOLER_DEFAULT			  0
#define COOLER_MAX				  3

#define HEAT_MAX	 			100
#define HEAT_RECOVERY_RATE		 13

#define HP_DEFAULT				  3
#define HP_MAX					  5

#define SHIELD_DEFAULT			  3
#define SHIELD_MAX				  6
#define SHIELD_RECOVERY_RATE	  0.3 //boules /sec.

#define TIMED_BONUS_DURATION	 10


PlayerShip::PlayerShip(const sf::Vector2f& offset) :
	Entity(GET_IMG("spaceship"), offset),
	controls_(AC::GetInstance()),
	panel_(ControlPanel::GetInstance()),
	laserbeam_(Weapon::LASERBEAM, this),
	hellfire_(Weapon::HELLFIRE, this)
{
#ifdef DEBUG
	puts("PlayerShip()");
#endif
	hp_ = HP_DEFAULT;
	shield_ = SHIELD_DEFAULT;
	coolers_ = COOLER_DEFAULT;
	overheated_ = false;
	shield_timer_ = 0;
	heat_ = 0.0f;
	speed_ = DEFAULT_SPEED;
#ifndef NO_AUDIO
	shield_sfx_.SetBuffer(GET_SOUNDBUF("warp"));
#endif

	ParticleSystem::GetInstance().AddShield(SHIELD_DEFAULT, &sprite_);
	
	panel_.SetMaxShipHP(HP_MAX);
	panel_.SetMaxShield(SHIELD_MAX);
	panel_.SetMaxHeat(HEAT_MAX);
	
	panel_.SetShipHP(hp_);
	panel_.SetShield(shield_);
	panel_.SetHeat(static_cast<int>(heat_));

	panel_.SetCoolers(coolers_);
	panel_.SetInfo("");
	
	// init timed bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		bonus_[i] = 0;
	}
	
#ifdef DEBUG
	puts("\t OK");
#endif
}


PlayerShip::~PlayerShip()
{
#ifdef DEBUG
	puts("~PlayerShip()");
#endif
}


void PlayerShip::HandleAction(AC::Action action)
{
	if (action == AC::USE_COOLER)
	{
		if (coolers_ > 0)
		{
			--coolers_;
			panel_.SetCoolers(coolers_);
			heat_ = 0.f;
			overheated_ = false;
			panel_.SetInfo("");
		}
	}
#ifdef DEBUG
	if (action == AC::USE_HACK)
	{
		hp_ = 30;
		panel_.SetShipHP(hp_);
		shield_ = 30;
		panel_.SetShield(shield_);
		ParticleSystem::GetInstance().AddMessage(sprite_.GetPosition(), L"Secret Haxxx Of The Doom");
	}
#endif
}


void PlayerShip::Action()
{

	if (!overheated_)
	{
		float h = 0.0f;
		sf::Vector2f offset = sprite_.GetPosition() + GUN_OFFSET;
/*#ifdef JOYSTICK_ENABLED
		if ((input_.IsJoystickButtonDown(1, binds_.Jweapon_a)) || (input_.IsKeyDown(binds_.weapon_a)) )
#else
		if (input_.IsKeyDown(binds_.weapon_a))
#endif*/
		if (controls_.HasInput(AC::WEAPON_1))
		{ 
			h += laserbeam_.Shoot(offset);
		}
/*#ifdef JOYSTICK_ENABLED
		if ((input_.IsJoystickButtonDown(1, binds_.Jweapon_b)) || (input_.IsKeyDown(binds_.weapon_b)) )
#else
		if (input_.IsKeyDown(binds_.weapon_b))
#endif*/
		if (controls_.HasInput(AC::WEAPON_2))
		{
			h += hellfire_.Shoot(offset);
		}

		
		heat_ += h;
		if (heat_ >= HEAT_MAX)
		{
			overheated_ = true;
			panel_.SetInfo("Surchauffe !");
		}
	}
}


void PlayerShip::Move(float frametime)
{
	static const float WIDTH = sprite_.GetSize().x;
	static const float HEIGHT = sprite_.GetSize().y;
	// déplacement
	const sf::Vector2f& offset = GetPosition();
	float x = offset.x;
	float y = offset.y;
	
	float dist = frametime * speed_;
	
	if (controls_.HasInput(AC::MOVE_UP))
	{
		y = (y - dist < CONTROL_PANEL_HEIGHT) ? CONTROL_PANEL_HEIGHT : y - dist;
	}
	if (controls_.HasInput(AC::MOVE_DOWN))
	{
		y = (y + HEIGHT + dist > WIN_HEIGHT) ? WIN_HEIGHT - HEIGHT : y + dist;
	}
	if (controls_.HasInput(AC::MOVE_LEFT))
	{
		x = (x - dist < 0) ? 0 : x - dist;
	}
	if (controls_.HasInput(AC::MOVE_RIGHT))
	{
		x = (x + WIDTH + dist > WIN_WIDTH) ? WIN_WIDTH - WIDTH : x + dist;
	}
	sprite_.SetPosition(x, y);
	
	// regénération bouclier
	if (shield_ < SHIELD_MAX)
	{
		shield_timer_ -= frametime;
		if (shield_timer_ <= 0.f)
		{
			// ajout d'une boule supplémentaire
			++shield_;
			ParticleSystem& p = ParticleSystem::GetInstance();
			p.RemoveShield(&sprite_);
			p.AddShield(shield_, &sprite_);
			
			shield_timer_ = 1 / SHIELD_RECOVERY_RATE;
			panel_.SetShield(shield_);
		}
	}
	
	// refroidissement
	if (heat_ > 0.f)
	{
		heat_ -= HEAT_RECOVERY_RATE * frametime;
		if (heat_ <= 0.f)
		{
			heat_ = 0.f;
			if (overheated_ )
			{
				overheated_ = false;
				panel_.SetInfo("");
			}
		}
	}

	panel_.SetHeat(static_cast<int>(heat_));
	
	// timer bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		if (bonus_[i] > 0)
		{
			bonus_[i] -= frametime;
			if (bonus_[i] <= 0)
			{
				bonus_[i] = 0;
				DisableTimedBonus((TimedBonus) i);
			}
		}
	}
	laserbeam_.Update(frametime);
	hellfire_.Update(frametime);
}


void PlayerShip::Hit(int damage)
{
	static ParticleSystem& p = ParticleSystem::GetInstance();
	if (shield_ > 0)
	{
		shield_ -= damage;
		p.RemoveShield(&sprite_);
		if (shield_ > 0)
		{
			p.AddShield(shield_, &sprite_);
		}
		else
		{
#ifndef NO_AUDIO
			shield_sfx_.Play();
#endif
			shield_ = 0;
		}
		panel_.SetShield(shield_);
	}
	else
	{
		Entity::Hit(damage);
		panel_.SetShipHP(hp_);
	}
}


void PlayerShip::Collide(Entity& ent)
{
	Bonus* bonus = dynamic_cast<Bonus*>(&ent);
	if (bonus != NULL)
	{
		HandleBonus(*bonus);
	}
	else
	{
		Hit(1);
	}
}


void PlayerShip::HandleBonus(const Bonus& bonus)
{
	switch (bonus.GetType())
	{
		case Bonus::TRISHOT:
			if (bonus_[T_TRISHOT] == 0)
			{
				hellfire_.SetTriple(true);
				laserbeam_.SetTriple(true);
				puts("bonus triple tir activé");
			}
			else
			{
				puts("bonus triple tir relancé");
			}
			bonus_[T_TRISHOT] += TIMED_BONUS_DURATION;
			break;
		case Bonus::SPEED:
			if (bonus_[T_SPEED] == 0)
			{
				speed_ *= 2;
				puts("bonus speed activé");
			}
			else
			{
				puts("bonus speed relancé");
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			break;
		case Bonus::HEALTH:
			if (hp_ < HP_MAX)
			{
				++hp_;
				panel_.SetShipHP(hp_);				
			}
			break;
		case Bonus::COOLER:
			if (coolers_ < COOLER_MAX)
			{
				++coolers_;
				panel_.SetCoolers(coolers_);
			}
			break;
		default:
			break;
	}
	ParticleSystem::GetInstance().AddMessage(bonus.GetPosition(), bonus.WhatItIs());

}


void PlayerShip::DisableTimedBonus(TimedBonus tbonus)
{
	switch (tbonus)
	{
		case T_TRISHOT:
			hellfire_.SetTriple(false);
			laserbeam_.SetTriple(false);
			puts("bonus triple tir désactivé");
			break;
		case T_SPEED:
			speed_ = DEFAULT_SPEED;
			puts("bonus speed désactivé");
			break;
		default:
			assert(0);
	}
	bonus_[tbonus] = 0;
}
