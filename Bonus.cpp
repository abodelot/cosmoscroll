#include "Bonus.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"

#define SPEED 100


Bonus::Bonus(const sf::Vector2f& offset) :
    Entity(GET_IMG("bonus"), offset, 1)
{
}


void Bonus::Hit(int damage)
{
    (void) damage; // un bonus ne peut être détruit
}


void Bonus::Move(float frametime)
{
    sprite_.Move(-SPEED * frametime, 0);
    if (outside_universe(GetRect()))
    {
        Kill();
    }
}

