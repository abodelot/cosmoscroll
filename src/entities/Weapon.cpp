#include "Weapon.hpp"
#include "BulletManager.hpp"


BulletManager& Weapon::bullets_ = BulletManager::GetInstance();


Weapon::Weapon(const sf::Image& image, float fire_rate, float heat_cost, int damage, int speed)
{
	fire_timer_ = 0.f;
	fire_rate_ = 1 / fire_rate;
	triple_ = false;
	owner_ = NULL;

	image_ = &image;
	heat_cost_ = heat_cost;
	damage_ = damage;
	speed_ = speed;
}


void Weapon::SetOwner(Entity* owner)
{
	owner_ = owner;
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
		int emitter = owner_->GetID();
		bullets_.Add(emitter, offset, angle, image_, speed_, damage_);
		if (triple_)
		{
			bullets_.Add(emitter, offset, angle - 0.15, image_, speed_, damage_);
			bullets_.Add(emitter, offset, angle + 0.15, image_, speed_, damage_);
		}
		fire_timer_ = fire_rate_;
		return heat_cost_;
	}
	return 0.f;
}


void Weapon::SetTriple(bool triple)
{
	triple_ = triple;
}

