#include "Blorb2.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"

#define GUN_OFFSET        sf::Vector2f(-1, 14)
#define GUN_OFFSET_INVERT sf::Vector2f(40, 14)


Blorb2::Blorb2(const sf::Vector2f& offset, Entity** target):
    Blorb(offset, target)
{
    // un peu crade car le constructeur de Blorb aura mis "capsule" par défaut...
    sprite_.SetImage(GET_IMG("WIP_interceptor"));
}


void Blorb2::Action()
{
    float radians = ANGLE((**target_).GetPosition(), sprite_.GetPosition());
    if (old_orient_)
    {
        weapon_.Shoot(sprite_.GetPosition() + GUN_OFFSET_INVERT, radians);
    }
    else
    {
        weapon_.Shoot(sprite_.GetPosition() + GUN_OFFSET, radians);
    }
}

