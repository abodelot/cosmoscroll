#include <cassert>
#include <typeinfo>
#include <SFML/System.hpp>

#include "PlayerShip.hpp"
#include "EntityManager.hpp"
#include "../core/Window.hpp"
#include "../core/Game.hpp"
#include "../core/ParticleSystem.hpp"
#include "../core/SoundSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"

#ifdef DEBUG
#include <iostream>
#endif

#define WEAPON1_OFFSET          52, 22
#define WEAPON2_OFFSET         50, 24

#define DEFAULT_SPEED           200
#define SPEED_BONUS_FACTOR      2

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


PlayerShip::PlayerShip(const sf::Vector2f& position, const char* animation) :
	Entity(position, HP_DEFAULT),
	Animated(EntityManager::GetInstance().GetAnimation(animation), *this),
	controller_(AC::GetInstance()),
	panel_(ControlPanel::GetInstance())
{
	SetTeam(Entity::GOOD);
	SetSubRect(GetAnimation().GetFrame(0)); // WTF
	// init weapons
	EntityManager& mgr = EntityManager::GetInstance();
	mgr.InitWeapon(1, &laserbeam_);
	laserbeam_.SetOwner(this);
	laserbeam_.SetOffset(WEAPON1_OFFSET);
	mgr.InitWeapon(2, &hellfire_);
	hellfire_.SetOwner(this);
	hellfire_.SetOffset(WEAPON2_OFFSET);

	shield_ = SHIELD_DEFAULT;
	coolers_ = COOLER_DEFAULT;
	overheated_ = false;
	shield_timer_ = 0;
	heat_ = 0.0f;
	speed_ = DEFAULT_SPEED;
	controls_ = AC::ALL;
	ParticleSystem::GetInstance().AddShield(SHIELD_DEFAULT, this);

	// init timed bonus
	for (int i = 0; i < TIMED_BONUS_COUNT; ++i)
	{
		bonus_[i] = 0.f;
	}

	// init control panel
	panel_.SetMaxShipHP(HP_MAX);
	panel_.SetMaxShield(SHIELD_MAX);
	panel_.SetMaxHeat(HEAT_MAX);
	panel_.SetShipHP(hp_);
	panel_.SetShield(shield_);
	panel_.SetHeat((int) heat_);
	panel_.SetCoolers(coolers_);
	panel_.SetInfo("");

	// init Konami code
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

#ifdef DEBUG
	printf("PlayerShip created at %p\n", (void*) this);
#endif
}


PlayerShip::~PlayerShip()
{
#ifdef DEBUG
	puts("PlayerShip deleted");
#endif
}


PlayerShip* PlayerShip::Clone() const
{
	return new PlayerShip(*this);
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


void PlayerShip::Update(float frametime)
{
	// animation
	Animated::Update(frametime, *this);

	if (!overheated_)
	{
		float h = 0.0f;
		if (controller_.HasInput(AC::WEAPON_1, controls_))
		{
			h += hellfire_.Shoot(GetPosition());
		}
		if (controller_.HasInput(AC::WEAPON_2, controls_))
		{
			h += laserbeam_.Shoot(GetPosition());
		}

		heat_ += h;
		if (heat_ >= HEAT_MAX)
		{
			overheated_ = true;
			panel_.SetInfo("Surchauffe !");
		}
	}
	// déplacement
	const sf::Vector2f& offset = GetPosition();
	float x = offset.x;
	float y = offset.y;
	static const int X_BOUND = WIN_WIDTH - GetSize().x;
	static const int Y_BOUND = WIN_HEIGHT - ControlPanel::HEIGHT - GetSize().y;
	if (controller_.HasInput(AC::MOVE_UP, controls_))
	{
		y = y - frametime * speed_;
	}
	if (controller_.HasInput(AC::MOVE_DOWN, controls_))
	{
		y = y + frametime * speed_;
	}
	if (controller_.HasInput(AC::MOVE_LEFT, controls_))
	{
		x = x - frametime * speed_;
	}
	if (controller_.HasInput(AC::MOVE_RIGHT, controls_))
	{
		x = x + frametime * speed_;
	}

	if (y < 0)
	{
		y = 0;
	}
	else if (y > Y_BOUND)
	{
		y = Y_BOUND;
	}
	if (x < 0)
	{
		x = 0;
	}
	else if (x > X_BOUND)
	{
		x = X_BOUND;
	}
	SetPosition(x, y);

	// regénération bouclier
	if (shield_ < SHIELD_MAX)
	{
		shield_timer_ -= frametime;
		if (shield_timer_ <= 0.f)
		{
			IncreaseShield();
			shield_timer_ = 1 / SHIELD_RECOVERY_RATE;
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


void PlayerShip::TakeDamage(int damage)
{
	static ParticleSystem& p = ParticleSystem::GetInstance();
	if (shield_ > 0)
	{
		shield_ -= damage;
		p.RemoveShield(this);
		if (shield_ > 0)
		{
			p.AddShield(shield_, this);
		}
		else
		{
			SoundSystem::GetInstance().PlaySound("warp");
			shield_ = 0;
		}
		panel_.SetShield(shield_);
	}
	else
	{
		Entity::TakeDamage(damage);
		panel_.SetShipHP(hp_);
		if (IsDead())
		{
			p.AddExplosion(GetPosition());
			Game::GetInstance().NotifyPlayerDead();
		}
	}
}


void PlayerShip::OnCollide(Entity& entity)
{
	Bonus* bonus = dynamic_cast<Bonus*>(&entity);
	if (bonus != NULL)
	{
		HandleBonus(*bonus);
		entity.Kill();
	}
	else
	{
		Entity::OnCollide(entity);
	}
}


bool PlayerShip::PixelPerfectCollide() const
{
	return true;
}


void PlayerShip::HandleBonus(const Bonus& bonus)
{
	switch (bonus.GetType())
	{
		// timed bonus
		case Bonus::TRIPLE_SHOT:
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
				speed_ *= SPEED_BONUS_FACTOR;
				printf("bonus speed activé, speed=%d\n", speed_);
			}
			else
			{
				printf("bonus speed relancé, speed=%d\n", speed_);
			}
			bonus_[T_SPEED] += TIMED_BONUS_DURATION;
			break;
		case Bonus::STONED:
			if (bonus_[T_STONED] == 0)
			{
				speed_ *= -1;
				printf("bonus stoned activé, speed=%d\n", speed_);
			}
			else
			{
				printf("bonus stoned relancé, speed=%d\n", speed_);
			}
			bonus_[T_STONED] += TIMED_BONUS_DURATION;
			break;

		// other bonus
		case Bonus::HEALTH:
			if (hp_ < HP_MAX)
			{
				++hp_;
				panel_.SetShipHP(hp_);
			}
			break;
		case Bonus::SHIELD:
			if (shield_ < SHIELD_MAX)
			{
				IncreaseShield();
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
			puts("info: bonus triple tir désactivé");
			break;
		case T_SPEED:
			speed_ /= SPEED_BONUS_FACTOR;
			printf("info: bonus speed désactivé, speed=%d\n", speed_);
			break;
		case T_STONED:
			speed_ *= -1;
			printf("info: bonus stoned désactivé, speed=%d\n", speed_);
			break;
		default:
			abort();
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
	particles.RemoveShield(this);
	particles.AddShield(shield_, this);
	hellfire_.SetTriple(true);
	laserbeam_.SetTriple(true);
	particles.AddMessage(GetPosition(), L"Have you mooed today?");
}


void PlayerShip::IncreaseShield(int count)
{
	shield_ += count;
	// update particles
	ParticleSystem& p = ParticleSystem::GetInstance();
	p.RemoveShield(this);
	p.AddShield(shield_, this);
	// update panel count
	panel_.SetShield(shield_);
}
