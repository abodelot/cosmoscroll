#include "Projectile.hpp"
#include "EntityManager.hpp"
#include "utils/Math.hpp"
#include "utils/StringUtils.hpp"


Projectile::Projectile(Entity* emitter, float angle, const sf::Texture& texture, int speed, int damage):
    m_damage(damage)
{
    setTexture(texture);
    setTeam(emitter->getTeam());
    setRotation(-math::to_degrees(angle));

    // Compute constant speed vector from velocity and angle
    m_speed.x = std::cos(angle) * speed;
    m_speed.y = -std::sin(angle) * speed;

    // Set origin at sprite's center to allow rotation
    setOrigin(getWidth() / 2, getHeight() / 2);
}

void Projectile::collides(Entity& entity)
{
    entity.onCollision(*this);
}

int Projectile::getDamage() const
{
    return m_damage;
}

void Projectile::onUpdate(float frametime)
{
    move(m_speed.x * frametime, m_speed.y * frametime);
}

void Projectile::onTileCollision()
{
    kill();
    EntityManager::getInstance().createImpactParticles(getPosition(), 4);
}
