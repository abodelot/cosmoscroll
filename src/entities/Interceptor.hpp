#ifndef INTERCEPTOR_HPP
#define INTERCEPTOR_HPP

#include "Ennemy.hpp"

class Interceptor: public Ennemy
{
public:
    Interceptor(const sf::Vector2f& offset, Entity* target);
    
    void Action();
    void Move(float frametime);
private:
    Weapon weapon_;
};

#endif /* guard INTERCEPTOR_HPP */

