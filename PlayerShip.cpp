#include "PlayerShip.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "ParticleSystem.hpp"
#include "Math.hpp"
#include "Settings.hpp"

#include <SFML/System.hpp>
#include <cassert>
#include <typeinfo>

#ifdef DEBUG
#include <iostream>
#endif

#define SHIP_SPEED      200
#define GUN_OFFSET      sf::Vector2f(52, 24)

// taux de regénération du bouclier en boules/secondes
#define SHIELD_RECOVERY_RATE 0.3
#define SHIELD_MAX           6
#define SHIELD_DEFAULT       3

#define COOLER_MAX           3
#define COOLER_DEFAULT       0

#define TRIGUN_DURATION     10

#define HEAT_MAX     100
#define COLDING_RATE 13

#define HP_DEFAULT   3
#define HP_MAX       5


PlayerShip::PlayerShip(const sf::Vector2f& offset, const sf::Input& input) :
    Entity(GET_IMG("spaceship"), offset),
    panel_(ControlPanel::GetInstance()),
    input_(input),
    laserbeam_(Weapon::LASERBEAM, this),
    hellfire_(Weapon::HELLFIRE, this)
{
#ifdef DEBUG
	puts("PlayerShip()");
#endif
	hp_ = HP_DEFAULT;
    overheated_ = false;
    heat_ = 0.0f;
    shield_ = SHIELD_DEFAULT;
	coolers_ = COOLER_DEFAULT;
	pause_effects_ = false;
    shield_timer_ = 0;
#ifndef NO_AUDIO
    shield_sfx_.SetBuffer(GET_SOUNDBUF("warp"));
#endif

	SetTimer(0);

    ParticleSystem::GetInstance().AddShield(SHIELD_DEFAULT, &sprite_);

    panel_.SetMaxShipHP(HP_MAX);
    panel_.SetMaxShield(SHIELD_MAX);
    panel_.SetMaxHeat(HEAT_MAX);
    
    panel_.SetShipHP(hp_);
    panel_.SetShield(shield_);
    panel_.SetHeat(static_cast<int>(heat_));
	panel_.SetCoolers(coolers_);
    panel_.SetInfo("");
    
    // init bindings config
    const Settings& settings = Settings::GetInstance();
    binds_.up = settings.GetKey(Settings::UP);
	binds_.down = settings.GetKey(Settings::DOWN);
	binds_.left = settings.GetKey(Settings::LEFT);
	binds_.right = settings.GetKey(Settings::RIGHT);
	binds_.weapon_a = settings.GetKey(Settings::WEAPON1);
	binds_.weapon_b = settings.GetKey(Settings::WEAPON2);
	binds_.bonus_cooler = settings.GetKey(Settings::BONUS_COOLER);
}


PlayerShip::~PlayerShip()
{
#ifdef DEBUG
	puts("~PlayerShip()");
#endif
    if (GetTimer() != 0)
    {
		Neutralize();
	}
}


void PlayerShip::HandleKey(const sf::Event::KeyEvent& key)
{
	if (key.Code == binds_.bonus_cooler)
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
	if (input_.IsKeyDown(sf::Key::H))
	{
		hp_ = 100;
		panel_.SetShipHP(hp_);
		ParticleSystem::GetInstance().AddMessage(sprite_.GetPosition(), L"Haxxx 100 HP !!");
	}
#endif
}


void PlayerShip::Action()
{
    if (!overheated_)
    {
        float h = 0.0f;
        sf::Vector2f offset = sprite_.GetPosition() + GUN_OFFSET;
        if (input_.IsKeyDown(binds_.weapon_a))
        { 
            h += laserbeam_.Shoot(offset);
        }
        if (input_.IsKeyDown(binds_.weapon_b))
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
    
    float dist = frametime * SHIP_SPEED;
    
    if (input_.IsKeyDown(binds_.up))
    {
        y = (y - dist < CONTROL_PANEL_HEIGHT) ? CONTROL_PANEL_HEIGHT : y - dist;
    }
    if (input_.IsKeyDown(binds_.down))
    {
        y = (y + HEIGHT + dist > WIN_HEIGHT) ? WIN_HEIGHT - HEIGHT : y + dist;
    }
    if (input_.IsKeyDown(binds_.left))
    {
        x = (x - dist < 0) ? 0 : x - dist;
    }
    if (input_.IsKeyDown(binds_.right))
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
        heat_ -= COLDING_RATE * frametime;
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
		if ((hp_ == 0) && (GetTimer() != 0))
		{
			Neutralize();
		}
        panel_.SetShipHP(hp_);
    }
}


void PlayerShip::Collide(Entity& ent)
{
    if (typeid (ent) == typeid (Bonus))
    {
        Bonus* bonus = dynamic_cast<Bonus*>(&ent);
        assert(bonus != NULL);
        HandleBonus(*bonus);
    }
    else
    {
        Hit(1);
		if ((hp_ == 0) && (GetTimer() != 0))
		{
			Neutralize();
		}
    }
}


void PlayerShip::EndBonusWrapper(void* data)
{
    PlayerShip* self = (PlayerShip*) data; 
	self->EndBonus();
}


void PlayerShip::EndBonus()
{
    SetTimer(TRIGUN_DURATION);
	thread_dead_ = false;
loop_back:
	while (!pause_effects_ && GetTimer() > 0)
    {
#ifdef DEBUG
#ifdef DEBUG_THREAD
        printf("\t-- %ds\n", GetTimer());
#endif
#endif
		GetMutex().Lock();
        --trigun_timer_;
		GetMutex().Unlock();
        sf::Sleep(1.0f);
    }
	if (!pause_effects_)
	{
		hellfire_.SetTriple(false);
		laserbeam_.SetTriple(false);
		thread_dead_ = true;
#ifdef DEBUG
		puts("\t-- fin bonus arme");
	#endif
	}
	else
	{
		while (pause_effects_)
		{
			sf::Sleep(1.0f);
		}
		goto loop_back;	// Le goto syndical, et il est utile :D
	}
}


void PlayerShip::HandleBonus(const Bonus& bonus)
{
    switch (bonus.GetType())
    {
        // bonus d'armes, tir triplé pour un laps de temps
        case Bonus::TRIGUN:
#ifdef DEBUG
			puts("\t got TRIGUN");	
#endif
            hellfire_.SetTriple(true);
            laserbeam_.SetTriple(true);
            // thread pour désactiver le bonus plus tard
            if (IsThDead())
            {
				GetThread().Launch();
            }
            else
            {
				GetMutex().Lock();
				trigun_timer_ += TRIGUN_DURATION;
				GetMutex().Unlock();
            }
            break;
        // bonus point de vie
        case Bonus::HEALTH:
#ifdef DEBUG
			puts("\t got HEALTH");	
#endif
		if (hp_ < HP_MAX)
			{
				++hp_;
				panel_.SetShipHP(hp_);				
			}
			break;
		case Bonus::COOLER:
#ifdef DEBUG
			puts("\t got COOLER");	
#endif
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

void PlayerShip::Neutralize()
{
#ifdef DEBUG
std::cerr << "\tNeutralization: ";
#endif
if(GetTimer())
	{
#ifdef DEBUG
	std::cerr << "necessaire:";
#endif
		SetTimer(0);
		GetThread().Terminate();
#ifdef DEBUG
		std::cerr << " effectuee.\n";
#endif
	}
	else
	{
#ifdef DEBUG	
		std::cerr << " inutile\n";
#endif
	}
}

