#include "Weapon.hpp"
#include "BulletManager.hpp"


BulletManager& Weapon::bullets_ = BulletManager::GetInstance();


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
			shot_per_second = 0.7f;
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


Weapon::~Weapon()
{
	bullets_.CleanSenders(owner_);
}


void Weapon::Update(float frametime)
{
	fire_timer_ -= frametime;
}


float Weapon::Shoot(const sf::Vector2f& offset, float angle)
{
	// peut-on tirer ?
	if (fire_timer_ <= 0.f)
	{
		bullets_.Add(type_, owner_, offset, angle);
		if (triple_)
		{
			bullets_.Add(type_, owner_, offset, angle - 0.15);
			bullets_.Add(type_, owner_, offset, angle + 0.15);
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

