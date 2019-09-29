#ifndef MISSILE_HPP
#define MISSILE_HPP

#include "Projectile.hpp"
#include "core/ParticleSystem.hpp"

class Missile: public Projectile
{
public:
    Missile(Entity* emitter, float angle, const sf::Texture& texture, int speed, int damage);

    ~Missile();

    // callbacks ---------------------------------------------------------------

    void onUpdate(float frametime) override;
    void onDestroy() override;

private:
    float   m_angle;
    Entity* m_owner;
    ParticleSystem::Emitter m_smoke_emitter;

};

#endif // MISSILE_HPP
