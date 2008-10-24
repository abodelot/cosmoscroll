#ifndef BLORB_HPP
#define BLORB_HPP

#include "Ennemy.hpp"

#include <SFML/Graphics.hpp>

/*
 * Un ennemy historique :)
 */
class Blorb: public Ennemy
{
public:
    Blorb(const sf::Vector2f& offset, Entity* target);
    
    void Action();
};

#endif /* guard BLORB_HPP */

