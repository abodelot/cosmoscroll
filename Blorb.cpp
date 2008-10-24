#include "Blorb.hpp"
#include "MediaManager.hpp"
#include "Math.hpp"

#define GUN_OFFSET        sf::Vector2f(-1, 14)


Blorb::Blorb(const sf::Vector2f& offset, Entity* target):
    Ennemy(offset, target)
{
    // crade
    sprite_.SetImage(GET_IMG("WIP_capsule"));
}


void Blorb::Action()
{
    float my_y = sprite_.GetPosition().y;
    float player_y = target_->GetPosition().y;
    // Doit on tirer ?
    if (std::abs(player_y - my_y) < 30)
    {
        sf::Vector2f offset = GUN_OFFSET; 
        float radians = PI;
        if (!left_)
        {
            // le premier pixel à la gauche de l'image
            offset.x = -1;
            radians *= -1;
        }
        weapon_.Shoot(sprite_.GetPosition() + offset, radians);
    }
}

