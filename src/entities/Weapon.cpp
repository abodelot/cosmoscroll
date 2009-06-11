#include "Weapon.hpp"
#include "EntityManager.hpp"
#include "Hit.hpp"

#include "../core/SoundSystem.hpp"


Weapon::Weapon(const sf::Image& image, float fire_rate, float heat_cost, int damage, int speed, const char* sound)
{
	fire_timer_ = 0.f;
	fire_rate_ = 1 / fire_rate;
	triple_ = false;
	owner_ = NULL;

	image_ = &image;
	heat_cost_ = heat_cost;
	damage_ = damage;
	speed_ = speed;
	sound_ = sound;

	x_ = 0;
	y_ = 0;
}


void Weapon::SetOffset(int x, int y)
{
	x_ = x;
	y_ = y;
}


void Weapon::SetOwner(Entity* owner)
{
	owner_ = owner;
}


void Weapon::Update(float frametime)
{
	fire_timer_ -= frametime;
}


float Weapon::Shoot(sf::Vector2f offset, float angle)
{
	static SoundSystem& sound_sys = SoundSystem::GetInstance();
	static EntityManager& entitymanager = EntityManager::GetInstance();

	// peut-on tirer ?
	if (fire_timer_ <= 0.f)
	{
		offset.x += x_;
		offset.y += y_;

		int emitter = owner_->GetID();
		entitymanager.AddEntity(new Hit(emitter, offset, angle, image_, speed_, damage_));
		if (triple_)
		{
			entitymanager.AddEntity(new Hit(emitter, offset, angle - 0.15, image_, speed_, damage_));
			entitymanager.AddEntity(new Hit(emitter, offset, angle + 0.15, image_, speed_, damage_));
		}
		fire_timer_ = fire_rate_;

		if (*sound_ != '\0')
		{
			sound_sys.PlaySound(sound_);
		}
		return heat_cost_;
	}
	return 0.f;
}


void Weapon::SetTriple(bool triple)
{
	triple_ = triple;
}

