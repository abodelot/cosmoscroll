#include "Weapon.hpp"
#include "items/ItemManager.hpp"
#include "EntityManager.hpp"


Weapon::Weapon():
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


void Weapon::init(const char* weapon_id, int level)
{
	ItemManager::GetInstance().InitWeapon(*this, weapon_id, level);
	m_inited = true;
}


bool Weapon::isInited() const
{
	return m_inited;
}


void Weapon::setTexture(const sf::Texture* texture)
{
	m_texture = texture;
}


void Weapon::setFireRate(float shot_per_sec)
{
	m_fire_delay = 1 / shot_per_sec;
}


void Weapon::setHeatCost(float heat_cost)
{
	m_heat_cost = heat_cost;
}


void Weapon::setDamage(int damage)
{
	m_damage = damage;
}


void Weapon::setVelociy(int velocity)
{
	m_velocity = velocity;
}


void Weapon::setSound(const sf::SoundBuffer* sound)
{
	m_sound = sound;
}


void Weapon::setPosition(const sf::Vector2f& pos)
{
	m_position = pos;
}


void Weapon::setOwner(Entity* owner)
{
	m_owner = owner;
}


bool Weapon::isReady() const
{
	return m_last_shot_at.getElapsedTime().asSeconds() >= m_fire_delay;
}


void Weapon::setMultiply(int n)
{
	m_multiply = n;
}


void Weapon::insert(const sf::Vector2f& pos, Entity* projectile)
{
	projectile->setPosition(pos);
	EntityManager::getInstance().addEntity(projectile);
}

