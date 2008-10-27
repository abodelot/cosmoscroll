#ifndef BLORB_HPP
#define BLORB_HPP

#include "Ennemy.hpp"

/*
 * Un ennemy historique :)
 */
class Blorb: public Ennemy
{
public:
    Blorb(const sf::Vector2f& offset, Entity* target);
    
    void Action();
    void Move(float frametime);
};

#endif /* guard BLORB_HPP */

