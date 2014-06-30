#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <stdexcept>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "Projectile.hpp"
#include "utils/Math.hpp"
#include "core/SoundSystem.hpp"

class Entity;


/**
 * Represents a weapon and can throw projectiles
 * Must be associated to an owner entity.
 */

class Weapon
{
public:
	Weapon();

	void init(const std::string& id);

	/**
	 * Throw a projectile
	 * @param angle: projectile direction (radians)
	 */
	template <class T = Projectile>
	float shoot(float angle);

	/**
	 * Throw a projectile
	 * @param target: targeted entity
	 */
	template <class T = Projectile>
	float shoot(const sf::Vector2f& target);

	/**
	 * Texture used for the projectiles
	 */
	void setTexture(const sf::Texture* texture);

	/**
	 * Projectile count per second
	 */
	void setFireRate(float shot_per_sec);

	/**
	 * Heat cost per projectile
	 */
	void setHeatCost(float heat_cost);

	/**
	 * Projectile damage on impact
	 */
	void setDamage(int damage);

	/**
	 * Projectile speed (pixels/sec)
	 */
	void setVelociy(int velocity);

	/**
	 * Sound effect play when shooting, NULL if no sound
	 */
	void setSound(const sf::SoundBuffer* sound);

	/**
	 * Weapon position, relative to owner's position
	 */
	void setPosition(int x, int y);

	/**
	 * Weapon's owner
	 */
	void setOwner(Entity* owner);

	/**
	 * true if weapon can be used
	 */
	bool isReady() const;

	void setMultiply(int n);

protected:
	template <class T = Projectile>
	void createProjectile(const sf::Vector2f& offset, float angle);

private:
	void insert(const sf::Vector2f& pos, Entity* entity);

	sf::Clock m_last_shot_at; // Store time for last shot
	float     m_fire_delay;   // Time to wait between next shot
	float     m_heat_cost;
	int       m_velocity;
	int       m_damage;
	const sf::Texture*     m_texture;
	const sf::SoundBuffer* m_sound;
	Entity*                m_owner;
	sf::Vector2f           m_position;
	int                    m_multiply;
};

template <class T>
float Weapon::shoot(float angle)
{
	static const float ANGLE_VARIATION = 0.15f;
	if (!m_texture)
		throw std::runtime_error("Using unitialized weapon");


	// If ready for next round
	if (m_last_shot_at.getElapsedTime().asSeconds() >= m_fire_delay)
	{
		sf::Vector2f pos = m_owner->getPosition() + m_position;

		switch (m_multiply)
		{
			case 1:
				createProjectile<T>(pos, angle);
				break;
			case 2:
				pos.y -= m_texture->getSize().y / 2 - 1;
				createProjectile<T>(pos, angle);
				pos.y += m_texture->getSize().y + 2;
				createProjectile(pos, angle);
				break;
			case 3:
				createProjectile<T>(pos, angle);
				createProjectile<T>(pos, angle - ANGLE_VARIATION);
				createProjectile<T>(pos, angle + ANGLE_VARIATION);
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
float Weapon::shoot(const sf::Vector2f& target)
{
	sf::Vector2f pos = m_owner->getPosition() + m_position;
	return shoot<T>(math::angle(pos, target));
}

template <class T>
void Weapon::createProjectile(const sf::Vector2f& position, float angle)
{
	T* projectile = new T(m_owner, angle, *m_texture, m_velocity, m_damage);
	insert(position, projectile);
}


#endif // WEAPON_HPP
