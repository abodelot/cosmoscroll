#include <cassert>
#include <cmath>

#include "Weapon.hpp"
#include "EntityManager.hpp"
#include "Hit.hpp"

#include "../core/SoundSystem.hpp"

#define ANGLE_VARIATION 0.15f


Weapon::Weapon()
{
	fire_timer_ = 0.f;
	fire_rate_ = 0;
	triple_ = false;
	owner_ = NULL;
	image_ = NULL;
	heat_cost_ = 0;
	damage_ = 0;
	speed_ = 0;
	sound_name_ = NULL;
	x_ = 0;
	y_ = 0;

	inited_ = false;
}


void Weapon::Init(const sf::Image& image, float fire_rate, float heat_cost,
	int damage, int speed)
{
	fire_rate_ = 1 / fire_rate;
	image_ = &image;
	heat_cost_ = heat_cost;
	damage_ = damage;
	speed_ = speed;

	inited_ = true;
}


bool Weapon::IsInited() const
{
	return inited_;
}


void Weapon::SetSoundName(const char* sound)
{
	sound_name_ = sound;
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


bool Weapon::IsReady() const
{
	return fire_timer_ <= 0;
}


float Weapon::Shoot(sf::Vector2f offset, float angle)
{
	assert(inited_);
	static SoundSystem& sound_sys = SoundSystem::GetInstance();

	// peut-on tirer ?
	if (fire_timer_ <= 0.f)
	{
		offset.x += x_;
		offset.y += y_;

		ThrowHit(offset, angle);

		if (triple_)
		{
			ThrowHit(offset, angle - ANGLE_VARIATION);
			ThrowHit(offset, angle + ANGLE_VARIATION);
		}
		fire_timer_ = fire_rate_;

		if (sound_name_ != NULL)
		{
			sound_sys.PlaySound(sound_name_);
		}
		return heat_cost_;
	}
	return 0.f;
}


void Weapon::SetTriple(bool triple)
{
	triple_ = triple;
}


void Weapon::ThrowHit(const sf::Vector2f& offset, float angle)
{
	static EntityManager& entitymanager = EntityManager::GetInstance();
	entitymanager.AddEntity(new Hit(owner_->GetTeam(), offset, angle, image_, speed_, damage_));
}


Entity* Weapon::GetOwner() const
{
	return owner_;
}
