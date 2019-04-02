#include "Missile.hpp"
#include "EntityManager.hpp"
#include "Player.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"


Missile::Missile(Entity* emitter, float angle, const sf::Texture& image, int speed, int damage):
    Projectile(emitter, angle, image, speed, damage),
    m_angle(angle),
    m_owner(emitter)
{
    m_smoke_emitter.setTextureRect(sf::IntRect(0, 0, 16, 16));
    m_smoke_emitter.setLooping(true);
    m_smoke_emitter.setSpeed(50, 25);
    m_smoke_emitter.setLifetime(1);
    m_smoke_emitter.setAngle(angle - math::PI, 0.5);
    m_smoke_emitter.setScale(0.5, 1.5);
    m_smoke_emitter.createParticles(80);
}


Missile::~Missile()
{
    m_smoke_emitter.clearParticles();
}


void Missile::onUpdate(float frametime)
{
    Projectile::onUpdate(frametime);
    m_smoke_emitter.setPosition(getX() - 6, getY() - 3);
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

