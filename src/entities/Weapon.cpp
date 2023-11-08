#include "Weapon.hpp"
#include "EntityManager.hpp"
#include "core/Services.hpp"


Weapon::Weapon():
    m_fire_delay(0.1f),
    m_heat_cost(0),
    m_velocity(0),
    m_damage(0),
    m_texture(nullptr),
    m_sound(nullptr),
    m_owner(nullptr),
    m_multiply(1)
{
}


void Weapon::init(const std::string& id)
{
    const Weapon& weapon = Services::getFactory().getWeapon(id);

    // Only copy attributes from XML document
    m_fire_delay = weapon.m_fire_delay;
    m_heat_cost = weapon.m_heat_cost;
    m_velocity = weapon.m_velocity;
    m_damage = weapon.m_damage;
    m_texture = weapon.m_texture;
    m_sound = weapon.m_sound;
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


void Weapon::setPosition(int x, int y)
{
    m_position = sf::Vector2f(x, y);
}


void Weapon::setOwner(Entity* owner)
{
    m_owner = owner;
}


bool Weapon::isReady() const
{
    return m_lastShotAt.getElapsedTime().asSeconds() >= m_fire_delay;
}


void Weapon::setMultiply(int n)
{
    m_multiply = n;
}


void Weapon::addProjectile(Projectile* projectile) const
{
    EntityManager::getInstance().addEntity(projectile);
}


void Weapon::playSound() const
{
    if (m_sound != nullptr) {
        Services::getSoundSystem().playSound(*m_sound);
    }
}
