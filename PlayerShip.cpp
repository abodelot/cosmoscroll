#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#include "PlayerShip.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "Math.hpp"
#include "ParticleSystem.hpp"

#ifdef DEBUG
#include <iostream>
#endif

#define GUN_OFFSET              sf::Vector2f(52, 24)

#define DEFAULT_SPEED           200
#define HIGH_SPEED              320

#define COOLER_DEFAULT          0
#define COOLER_MAX              3

#define HEAT_MAX                100
#define HEAT_RECOVERY_RATE      13

#define HP_DEFAULT              3
#define HP_MAX                  5

#define SHIELD_DEFAULT          3
#define SHIELD_MAX              6
#define SHIELD_RECOVERY_RATE    0.3 //boules /sec.

#define TIMED_BONUS_DURATION    10


PlayerShip::PlayerShip(const sf::Vector2f& offset, const char* image) :
	Entity(GET_IMG(image), offset),
	controller_(AC::GetInstance()),
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
		bonus_[i] = 0.f;
	}
	
	// init Konami Code
	konami_code_[0] = AC::MOVE_UP;
	konami_code_[1] = AC::MOVE_UP;
	konami_code_[2] = AC::MOVE_DOWN;
	konami_code_[3] = AC::MOVE_DOWN;
	konami_code_[4] = AC::MOVE_LEFT;
	konami_code_[5] = AC::MOVE_RIGHT;
	konami_code_[6] = AC::MOVE_LEFT;
	konami_code_[7] = AC::MOVE_RIGHT;
	konami_code_[8] = AC::WEAPON_2;
	konami_code_[9] = AC::WEAPON_1;
	current_konami_event_ = 0;
	
	controls_ = AC::ALL;
	use_limits_ = true;
}


PlayerShip::~PlayerShip()
{
#ifdef DEBUG
	puts("~PlayerShip()");
#endif
}


void PlayerShip::HandleAction(AC::Action action)
{
	if (action == AC::NONE)
	{
		return;
	}
	
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
	// konami code
	if (action == konami_code_[current_konami_event_])
	{
		++current_konami_event_;
		if (current_konami_event_ == KONAMI_CODE_LENGTH)
		{
			current_konami_event_ = 0;
			KonamiCodeOn();
		}
	}
	else
	{
		current_konami_event_ = 0;
	}

}


void PlayerShip::Action()
{
	if (!overheated_)
	{
		float h = 0.0f;
		sf::Vector2f offset = sprite_.GetPosition() + GUN_OFFSET;
		
		if (controller_.HasInput(AC::WEAPON_1, controls_))
		{ 
			h += hellfire_.Shoot(offset);
		}
		if (controller_.HasInput(AC::WEAPON_2, controls_))
		{
			h += laserbeam_.Shoot(offset);
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
	if (controller_.HasInput(AC::MOVE_UP, controls_))
	{
		/*if (use_limits_)
		{
			y = (y - dist < y_limits_.x) ? y_limits_.x : y - dist;
		}
		else
		{*/
			y = (y - dist < CONTROL_PANEL_HEIGHT) ? CONTROL_PANEL_HEIGHT : y - dist;
		/*}*/
	}
	if (controller_.HasInput(AC::MOVE_DOWN, controls_))
	{
		/*if (use_limits_)
		{
			y = (y + dist > y_limits_.y - HEIGHT) ? y_limits_.y - HEIGHT : y + dist;
		}
		else
		{*/
			y = (y + HEIGHT + dist > WIN_HEIGHT) ? WIN_HEIGHT - HEIGHT : y + dist;
		/*}*/
	}
	if (controller_.HasInput(AC::MOVE_LEFT, controls_))
	{
		/*if (use_limits_)
		{
			x = (x - dist < x_limits_.x) ? x_limits_.x : x - dist;
		}
		else
		{*/
			x = (x - dist < 0) ? 0 : x - dist;
		/*}*/
	}
	if (controller_.HasInput(AC::MOVE_RIGHT, controls_))
	{
		/*if (use_limits_)
		{
			x = (x + dist > x_limits_.y - WIDTH) ? x_limits_.y - WIDTH : x + dist;
		}
		else
		{*/
			x = (x + WIDTH + dist > WIN_WIDTH) ? WIN_WIDTH - WIDTH : x + dist;
		/*}*/
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
		if (IsDead())
		{
			p.AddExplosion(sprite_.GetPosition());
		}
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
				speed_ = HIGH_SPEED;
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
	ParticleSystem::GetInstance().AddMessage(bonus.GetPosition(),
		bonus.WhatItIs());
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


void PlayerShip::KonamiCodeOn()
{
	ParticleSystem& particles = ParticleSystem::GetInstance();
	overheated_ = false;
	panel_.SetInfo("");
	heat_ = 0.f;
	panel_.SetHeat((int) heat_);	
	coolers_ = 42;
	panel_.SetCoolers(42);
	hp_ = 42;
	panel_.SetShipHP(hp_);
	shield_ = 42;
	panel_.SetShield(shield_);
	particles.RemoveShield(&sprite_);
	particles.AddShield(shield_, &sprite_);
	hellfire_.SetTriple(true);
	laserbeam_.SetTriple(true);
	particles.AddMessage(sprite_.GetPosition(), L"Have you mooed today?");
}

