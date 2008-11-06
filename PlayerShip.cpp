#include "PlayerShip.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "ParticleSystem.hpp"
#include "Math.hpp"

#include <SFML/System.hpp>
#include <cassert>
#include <typeinfo>


#define SHIP_SPEED      200
#define GUN_OFFSET      sf::Vector2f(52, 24)

// taux de regénération du bouclier en boules/secondes
#define SHIELD_RECOVERY_RATE 0.2
#define SHIELD_MAX           6
#define SHIELD_DEFAULT       3

#define HEAT_MAX     100
#define COLDING_RATE 13

#define GUN_ORIENT_MAX  15
#define GUN_ORIENT_MIN -15

PlayerShip::PlayerShip(const sf::Vector2f& offset, const sf::Input& input) :
    Entity(GET_IMG("spaceship"), offset),
    panel_(ControlPanel::GetInstance()),
    input_(input),
    laserbeam_(Weapon::LASERBEAM),
    hellfire_(Weapon::HELLFIRE)
{
    is_lighten_ = false;
    overheated_ = false;
    heat_ = 0.0f;
    shield_ = SHIELD_DEFAULT;
    shield_timer_ = 0;
#ifndef NO_AUDIO
    shield_sfx_.SetBuffer(GET_SOUNDBUF("warp"));
#endif

    trigun_timer_ = false;

	
    ParticleSystem::GetInstance().AddShield(SHIELD_DEFAULT, &sprite_);
    
    panel_.SetShipHP(hp_);
    panel_.SetShield(shield_);
    panel_.SetHeat(heat_);
    panel_.SetInfo("");
}


PlayerShip::~PlayerShip()
{
    if (trigun_timer_ != 0)
    {
        trigun_timer_ = 0;
        GetTrigunThread().Wait();
    }

    puts("~PlayerShip()");
}


void PlayerShip::Action()
{
    if (is_lighten_)
    {
        sprite_.SetImage(GET_IMG("spaceship"));
        is_lighten_ = false;
    }
    
    if (!overheated_)
    {
        float h = 0.0f;
        sf::Vector2f offset = sprite_.GetPosition() + GUN_OFFSET;
        if (input_.IsKeyDown(sf::Key::Space))
        { 
            h += laserbeam_.Shoot(offset);
        }
        if (input_.IsKeyDown(sf::Key::A))
        {
            h += hellfire_.Shoot(offset);
        }
        
        if (h > 0.0f)
        {
            // si chaleur, alors il y a eu tir.
            // problème : si une arme peut tirer sans dégager de chaleur
            // (retourne toujours 0.f)
            sprite_.SetImage(GET_IMG("spaceship_light"));
            is_lighten_ = true;
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
    
    if (input_.IsKeyDown(sf::Key::Up))
    {
        y = (y - dist < CONTROL_PANEL_HEIGHT) ? CONTROL_PANEL_HEIGHT : y - dist;
    }
    if (input_.IsKeyDown(sf::Key::Down))
    {
        y = (y + HEIGHT + dist > WIN_HEIGHT) ? WIN_HEIGHT - HEIGHT : y + dist;
    }
    if (input_.IsKeyDown(sf::Key::Left))
    {
        x = (x - dist < 0) ? 0 : x - dist;
    }
    if (input_.IsKeyDown(sf::Key::Right))
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
    panel_.SetHeat(heat_);
    
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
        p.AddImpact(sprite_.GetPosition(), 10);
    }
    else
    {
        Entity::Hit(damage);
		
		if ((hp_ == 0) && (trigun_timer_ != 0))
		{
			trigun_timer_ = 0;
			GetTrigunThread().Wait();
		}
        panel_.SetShipHP(hp_);
        p.AddImpact(sprite_.GetPosition(), 20);
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
        --hp_;
		if ((hp_ == 0) && (trigun_timer_ != 0))
		{
			trigun_timer_ = 0;
			GetTrigunThread().Wait();
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
    trigun_timer_ = 10; // 10 secondes
    while (trigun_timer_ > 0)
    {
        printf("\t-- %ds\n", trigun_timer_);
        --trigun_timer_;
        sf::Sleep(1.0f);
    }
    hellfire_.SetTriple(false);
    laserbeam_.SetTriple(false);
    puts("\t-- fin bonus arme");
}


void PlayerShip::HandleBonus(const Bonus& bonus)
{
    switch (bonus.GetType())
    {
        // bonus d'armes, tir triplé pour un laps de temps
        case Bonus::TRIGUN:
            
            hellfire_.SetTriple(true);
            laserbeam_.SetTriple(true);
            // thread pour désactiver le bonus plus tard
            if (trigun_timer_ == 0)
            {
                puts("\t|bonus arme|");
                GetTrigunThread().Launch();
            }
            else
            {
                puts("\t|bonus arme relancé|");
                trigun_timer_ += 10;
            }
            break;
        // bonus point de vie
        case Bonus::HEALTH:
            puts("\t|bonus hp|");
            ++hp_;
            panel_.SetShipHP(hp_);
            break;
        default:
            break;
    }
    puts("exit handlebonus");
}




