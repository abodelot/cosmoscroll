#include "Ennemy.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"
#include "ParticleSystem.hpp"
#include "Window.hpp"

#include "Interceptor.hpp"
#include "Blorb.hpp"
#include "Drone.hpp"


Ennemy::Ennemy(const sf::Vector2f& offset, const sf::Image& img, int hp,
    Entity* target) :
    Entity(img, offset, hp)
{
    target_ = target;
}


void Ennemy::Hit(int damage)
{
    Entity::Hit(damage);
    if (IsDead())
    {
        ParticleSystem::GetInstance().AddExplosion(sprite_.GetPosition());   
    }
}


void Ennemy::Move(float frametime)
{
    if (outside_universe(GetRect()))
    {
        Kill();
    }
    (void) frametime;
}


Ennemy* Ennemy::Make(Type type, const sf::Vector2f& offset, Entity* target)
{
    switch (type)
    {
    case BLORB:
        return new Blorb(offset, target);
    case DRONE:
        return new Drone(offset, target);
    case INTERCEPTOR:
        return new Interceptor(offset, target);
    }
    return NULL;
}


