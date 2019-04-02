#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include "Entity.hpp"

/**
 * Bullet entity, see Weapon class
 */
class Projectile: public Entity
{
public:
    /**
     * @param emitter: entity which fired the projectile
     * @param angle: trajectory angle (radians)
     * @param texture: texture displayed
     * @param speed: velocity (pixels / seconde)
     * @param damage: inflicted damage if entity is damageable
     */
    Projectile(Entity* emitter, float angle, const sf::Texture& texture, int speed, int damage);

    void collides(Entity& entity);

    int getDamage() const;

    // callbacks ---------------------------------------------------------------

    void onUpdate(float frametime);

private:
    sf::Vector2f m_speed;
    int          m_damage;
};

#endif // PROJECTILE_HPP
