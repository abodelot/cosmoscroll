#include <cassert>
#include <cmath>

#include "entities/EntityManager.hpp"
#include "items/ItemManager.hpp"
#include "core/SoundSystem.hpp"
#include "utils/Math.hpp"

#define ANGLE_VARIATION 0.15f

template <class T>
Weapon<T>::Weapon():
	m_fire_delay(1.f),
	m_heat_cost(0),
	m_velocity(0),
	m_damage(0),
	m_texture(NULL),
	m_sound(NULL),
	m_owner(NULL),
	m_inited(false),
	m_multiply(1)
{
}

template <class T>
void Weapon<T>::init(const char* weapon_id, int level)
{
	ItemManager::GetInstance().InitWeapon(*this, weapon_id, level);
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

	// peut-on tirer ?
	if (m_last_shot_at.getElapsedTime().asSeconds() >= m_fire_delay)
	{
		sf::Vector2f offset = m_owner->getPosition() + m_position;

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
			SoundSystem::playSound(*m_sound);
		}
		m_last_shot_at.restart();
		return m_heat_cost;
	}
	return 0.f;
}

template <class T>
float Weapon<T>::shoot(const sf::Vector2f& target)
{
	sf::Vector2f pos = m_owner->getPosition() + m_position;
	return shoot(math::angle(pos, target));
}

template <class T>
void Weapon<T>::setTexture(const sf::Texture* texture)
{
	m_texture = texture;
}

template <class T>
void Weapon<T>::setFireRate(float shot_per_sec)
{
	m_fire_delay = 1 / shot_per_sec;
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
	m_position = pos;
}


template <class T>
void Weapon<T>::setOwner(Entity* owner)
{
	m_owner = owner;
}

template <class T>
bool Weapon<T>::isReady() const
{
	return m_last_shot_at.getElapsedTime().asSeconds() >= m_fire_delay;
}

template <class T>
void Weapon<T>::setMultiply(int n)
{
	m_multiply = n;
}

template <class T>
void Weapon<T>::createProjectile(const sf::Vector2f& position, float angle)
{
	T* projectile = new T(m_owner, angle, *m_texture, m_velocity, m_damage);
	projectile->setPosition(position);
	EntityManager::getInstance().addEntity(projectile);
}


