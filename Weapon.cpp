#include "Weapon.hpp"
#include "BulletManager.hpp"


Weapon::Weapon(Type type, Entity* sender)
{
    float shot_per_second;
    switch (type)
    {
        case LASERBEAM:
            shot_per_second = 8.f;
            energy_cost_ = 3.f;
            break;
        case HELLFIRE:
            shot_per_second = 24.f;
            energy_cost_ = 1.7f;
            break;
        case PLASMACANNON:
            shot_per_second = 3.f;
            energy_cost_ = 5.f;
            break;
		case DEVILSEYES:
			shot_per_second = 2.f;
			energy_cost_ = 1.f;
			break;
        default:
            exit(EXIT_FAILURE);
    }
    
    type_ = type;
    fire_timer_ = 0.f;
    fire_rate_ = 1 / shot_per_second;
    triple_ = false;
	owner_ = sender;
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
        bm.Add(type_, owner_, offset, angle);
        if (triple_)
        {
            bm.Add(type_, owner_, offset, angle - 0.15);
            bm.Add(type_, owner_, offset, angle + 0.15);
        }
        fire_timer_ = fire_rate_;
        return energy_cost_;
    }
    return 0.f;
}


void Weapon::SetTriple(bool triple)
{
    triple_ = triple;
}

