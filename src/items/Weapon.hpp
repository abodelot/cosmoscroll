#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include "entities/Projectile.hpp"

class Entity;


/**
 * Represents a weapon and can throw bullet entities in the entity manager.
 * Must be associated to an owner entity.
 */
template <class T = Projectile>
class Weapon
{
public:
	Weapon();

	/**
	 * Init weapon settings
	 * @param weapon_id: see weapons.xml
	 */
	void init(const char* weapon_id);

	/**
	 * @return true if weapon has already been inited
	 */
	bool isInited() const;

	/**
	 * Throw a projectile
	 * @param angle: projectile direction (radians)
	 * @param pos: projectile target (angle will be computed)
	 */
	float shoot(float angle);
	float shoot(const sf::Vector2f& target);

	void onUpdate(float frametime);

	/**
	 * Texture used for the projectiles
	 */
	void setTexture(const sf::Texture* texture);

	/**
	 * Amount of projectiles per second
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
	void setPosition(const sf::Vector2f& position);
	void setPosition(float x, float y);

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
	void createProjectile(const sf::Vector2f& offset, float angle);



private:
	float m_fire_rate;
	// timer "compte à rebours" pour que la cadence de tir soit indépendante
	// du nombre de FPS. Si <= 0, on peut tirer. Si tir, timer reinitialisé.
	// le timer est mis à jour à chaque frame dans Update
	float m_fire_timer;
	float m_heat_cost;
	int   m_velocity;
	int   m_damage;
	const sf::Texture*     m_texture;
	const sf::SoundBuffer* m_sound;
	Entity* m_owner;
	bool m_inited;
	int m_x;
	int m_y;
	int m_multiply;
};

#include "Weapon.inl"

#endif // WEAPON_HPP
