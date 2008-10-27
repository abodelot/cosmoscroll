#include "Blorb.hpp"
#include "MediaManager.hpp"


#define SPEED 50


Blorb::Blorb(const sf::Vector2f& offset, Entity* target) :
    Ennemy(offset, GET_IMG("capsule"), 3, target)
{
}

void Blorb::Action()
{
    // VOID
}

void Blorb::Move(float frametime)
{
    sprite_.Move(-SPEED * frametime, 0);
    Ennemy::Move(frametime);
}

