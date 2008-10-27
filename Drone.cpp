#include "Drone.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"


#define GUN_OFFSET  sf::Vector2f(-1, 14)
#define SPEED       220

Drone::Drone(const sf::Vector2f& offset, Entity* target) :
    Ennemy(offset, GET_IMG("ennemy-B"), 4, target),
    weapon_(Weapon::PLASMACANNON)
{
}


void Drone::Move(float frametime)
{
    sprite_.Move(-SPEED * frametime, 0);
    Ennemy::Move(frametime);
}


void Drone::Action()
{
    float my_y = sprite_.GetPosition().y;
    float player_y = target_->GetPosition().y;
    // Doit on tirer ?
    if (std::abs(player_y - my_y) < 30)
    {
        weapon_.Shoot(sprite_.GetPosition() + GUN_OFFSET, -PI);
    }
}

