#ifndef DRONE_HPP
#define DRONE_HPP

#include "Ennemy.hpp"

class Drone: public Ennemy
{
public:
    Drone(const sf::Vector2f& offset, Entity* target);
    
    void Action();
    void Move(float frametime);
private:
    Weapon weapon_;
};

#endif /* guard DRONE_HPP */

