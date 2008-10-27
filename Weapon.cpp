#include "Weapon.hpp"
#include "BulletManager.hpp"
#include "MediaManager.hpp"

Weapon::Weapon(Type type)
{
    float shot_per_second;
    switch (type)
    {
        case LASERBEAM:
            shot_per_second = 8.f;
            energy_cost_ = 3.f;
#ifndef NO_AUDIO
            sound_.SetBuffer(GET_SOUNDBUF("blaster-shot-1"));
#endif
            break;
        case HELLFIRE:
            shot_per_second = 24.f;
            energy_cost_ = 1.7f;
#ifndef NO_AUDIO
            sound_.SetBuffer(GET_SOUNDBUF("blaster-shot-2"));
#endif
            break;
        case PLASMACANNON:
            shot_per_second = 3.f;
            energy_cost_ = 5.f;
            break;
        default:
            exit(EXIT_FAILURE);
    }
    
    type_ = type;
    fire_timer_ = 0.f;
    fire_rate_ = 1 / shot_per_second;
}


void Weapon::Update(float frametime)
{
    fire_timer_ -= frametime;
}


float Weapon::Shoot(const sf::Vector2f& offset, float angle)
{
    static BulletManager& bm = BulletManager::GetInstance();
    // peut-on tirer ?
    if (fire_timer_ <= 0.f)
    {
        bm.Add(type_, offset, angle);
#ifndef NO_AUDIO
        sound_.Play();
#endif
        fire_timer_ = fire_rate_;
        return energy_cost_;
    }
    return 0.f;
}

