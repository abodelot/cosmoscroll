#include "Missile.hpp"
#include "EntityManager.hpp"
#include "Player.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"


Missile::Missile(Entity* emitter, float angle, const sf::Texture& texture, int speed, int damage):
    Projectile(emitter, angle, texture, speed, damage),
    m_angle(angle),
    m_owner(emitter)
{
    m_smokeEmitter.setTextureRect(sf::IntRect(0, 0, 16, 16));
    m_smokeEmitter.setLooping(true);
    m_smokeEmitter.setSpeed(50, 25);
    m_smokeEmitter.setLifetime(1);
    m_smokeEmitter.setAngle(angle - math::PI, 0.5);
    m_smokeEmitter.setScale(0.5, 1.5);
    m_smokeEmitter.createParticles(80);
}


Missile::~Missile()
{
    m_smokeEmitter.clearParticles();
}


void Missile::onUpdate(float frametime)
{
    Projectile::onUpdate(frametime);
    m_smokeEmitter.setPosition(getX() - 6, getY() - 3);
}


void Missile::onDestroy()
{
    for (int i = 0; i < 20; ++i)
    {
        float angle = math::rand(m_angle - math::PI / 2, m_angle + math::PI / 2);
        float speed = math::rand(200, 600);

        Projectile* p = new Projectile(m_owner, angle, Resources::getTexture("ammo/laser-red.png"), speed, 10);
        p->setPosition(getPosition());
        EntityManager::getInstance().addEntity(p);
    }
}

