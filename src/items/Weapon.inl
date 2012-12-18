#include <cassert>
#include <cmath>

#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "core/SoundSystem.hpp"
#include "utils/Math.hpp"

#define ANGLE_VARIATION 0.15f

template <class T>
Weapon<T>::Weapon():
	m_fire_rate(0),
	m_fire_timer(0.f),
	m_heat_cost(0),
	m_velocity(0),
	m_damage(0),
	m_texture(NULL),
	m_sound(NULL),
	m_owner(NULL),
	m_inited(false),
	m_x(0),
	m_y(0),
	m_multiply(1)
{
}

template <class T>
void Weapon<T>::init(const char* weapon_id)
{
	ItemManager::GetInstance().InitWeapon(weapon_id, *this);
	m_inited = true;
}

template <class T>
bool Weapon<T>::isInited() const
{
	return m_inited;
}

template <class T>
float Weapon<T>::shoot(float angle)
{
	assert(m_inited);
	static SoundSystem& sound_sys = SoundSystem::GetInstance();

	// peut-on tirer ?
	if (m_fire_timer <= 0.f)
	{
		sf::Vector2f offset = m_owner->getPosition();
		offset.x += m_x;
		offset.y += m_y;

		switch (m_multiply)
		{
			case 1:
				createProjectile(offset, angle);
				break;
			case 2:
				offset.y -= 5;
				createProjectile(offset, angle);
				offset.y += 10;
				createProjectile(offset, angle);
				break;
			case 3:
				createProjectile(offset, angle);
				createProjectile(offset, angle - ANGLE_VARIATION);
				createProjectile(offset, angle + ANGLE_VARIATION);
				break;
		}

		if (m_sound != NULL)
		{
			sound_sys.PlaySound(*m_sound);
		}
		m_fire_timer = m_fire_rate;
		return m_heat_cost;
	}
	return 0.f;
}

template <class T>
float Weapon<T>::shoot(const sf::Vector2f& target)
{
	sf::Vector2f pos = m_owner->getPosition();
	pos.x += m_x;
	pos.y += m_y;
	return shoot(math::angle(target, pos));
}

template <class T>
void Weapon<T>::onUpdate(float frametime)
{
	m_fire_timer -= frametime;
}

template <class T>
void Weapon<T>::setTexture(const sf::Texture* texture)
{
	m_texture = texture;
}

template <class T>
void Weapon<T>::setFireRate(float shot_per_sec)
{
	m_fire_rate = 1 / shot_per_sec;
}

template <class T>
void Weapon<T>::setHeatCost(float heat_cost)
{
	m_heat_cost = heat_cost;
}

template <class T>
void Weapon<T>::setDamage(int damage)
{
	m_damage = damage;
}

template <class T>
void Weapon<T>::setVelociy(int velocity)
{
	m_velocity = velocity;
}

template <class T>
void Weapon<T>::setSound(const sf::SoundBuffer* sound)
{
	m_sound = sound;
}

template <class T>
void Weapon<T>::setPosition(const sf::Vector2f& pos)
{
	m_x = pos.x;
	m_y = pos.y;
}

template <class T>
void Weapon<T>::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

template <class T>
void Weapon<T>::setOwner(Entity* owner)
{
	m_owner = owner;
}

template <class T>
bool Weapon<T>::isReady() const
{
	return m_fire_timer <= 0;
}

template <class T>
void Weapon<T>::setMultiply(int n)
{
	m_multiply = n;
}

template <class T>
void Weapon<T>::createProjectile(const sf::Vector2f& position, float angle)
{
	T* projectile = new T(m_owner, position, angle, m_texture, m_velocity, m_damage);
	EntityManager::GetInstance().AddEntity(projectile);
}


