#ifndef HIT_HPP
#define HIT_HPP

#include "Entity.hpp"

/**
 * Bullet entity, emitted by Weapon class
 */
class Hit: public Entity
{
public:
	/**
	 * @param emitter: entity which fired the projectile
	 * @param position: initial position
	 * @param angle: trajectory angle (radians)
	 * @param image: image du projectile
	 * @param speed: vitesse du projectile (pixels / seconde)
	 * @param damage: dégâts infligés lors d'une collision
	 */
	Hit(Entity* emitter, const sf::Vector2f& position, float angle,
		const sf::Texture* image, int speed, int damage);

	// inherited
	Hit* Clone() const;

	// inherited
	void Update(float frametime);

	// inherited
	void OnCollide(Entity& entity);

private:
	sf::Vector2f speed_;
};

#endif // HIT_HPP
