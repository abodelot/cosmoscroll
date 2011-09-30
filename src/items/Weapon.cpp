#include <cassert>
#include <cmath>

#include "Weapon.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Hit.hpp"
#include "items/ItemManager.hpp"
#include "core/SoundSystem.hpp"
#include "utils/Math.hpp"

#define ANGLE_VARIATION 0.15f


Weapon::Weapon()
{
	fire_timer_ = 0.f;
	fire_rate_ = 0;
	multiply_ = 1;
	owner_ = NULL;
	image_ = NULL;
	heat_cost_ = 0;
	damage_ = 0;
	velocity_ = 0;
	sound_name_ = NULL;
	x_ = 0;
	y_ = 0;

	inited_ = false;
}


void Weapon::Init(const char* weapon_id)
{
	ItemManager::GetInstance().InitWeapon(weapon_id, this);
	inited_ = true;
}


bool Weapon::IsInited() const
{
	return inited_;
}


float Weapon::Shoot(float angle)
{
	assert(inited_);
	static SoundSystem& sound_sys = SoundSystem::GetInstance();

	// peut-on tirer ?
	if (fire_timer_ <= 0.f)
	{
		sf::Vector2f offset = owner_->GetPosition();
		offset.x += x_;
		offset.y += y_;

		switch (multiply_)
		{
			case 1:
				ThrowHit(offset, angle);
				break;
			case 2:
				offset.y -= 5;
				ThrowHit(offset, angle);
				offset.y += 10;
				ThrowHit(offset, angle);
				break;
			case 3:
				ThrowHit(offset, angle);
				ThrowHit(offset, angle - ANGLE_VARIATION);
				ThrowHit(offset, angle + ANGLE_VARIATION);
				break;
		}

		if (sound_name_ != NULL)
		{
			sound_sys.PlaySound(sound_name_);
		}
		fire_timer_ = fire_rate_;
		return heat_cost_;
	}
	return 0.f;
}


float Weapon::ShootAt(const sf::Vector2f& target)
{
	sf::Vector2f my_pos = owner_->GetPosition();
	my_pos.x += x_;
	my_pos.y += y_;
	return Shoot(math::angle(target, my_pos));
}


void Weapon::Update(float frametime)
{
	fire_timer_ -= frametime;
}


void Weapon::SetImage(const sf::Image* image)
{
	image_ = image;
}

void Weapon::SetFireRate(float shot_per_sec)
{
	fire_rate_ = 1 / shot_per_sec;
}

void Weapon::SetHeatCost(float heat_cost)
{
	heat_cost_ = heat_cost;
}

void Weapon::SetDamage(int damage)
{
	damage_ = damage;
}

void Weapon::SetVelociy(int velocity)
{
	velocity_ = velocity;
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


void Weapon::SetOffset(const sf::Vector2f& offset)
{
	SetOffset(offset.x, offset.y);
}


void Weapon::SetOwner(Entity* owner)
{
	owner_ = owner;
}


bool Weapon::IsReady() const
{
	return fire_timer_ <= 0;
}


void Weapon::SetMultiply(int n)
{
	multiply_ = n;
}


void Weapon::ThrowHit(const sf::Vector2f& offset, float angle)
{
	static EntityManager& entitymanager = EntityManager::GetInstance();
	entitymanager.AddEntity(new Hit(owner_->GetTeam(), offset, angle, image_, velocity_, damage_));
}


Entity* Weapon::GetOwner() const
{
	return owner_;
}
