#ifndef H_BLORB_2
#define H_BLORB_2

#include "Blorb.hpp"

/*
 * Un blorb avec une image et un algo de tir différents
 * classe temporaire pour changer d'ennemi...
 */
class Blorb2: public Blorb
{
public:
    Blorb2(const sf::Vector2f& offset, Entity** target);
    
    void Action();
};

#endif /* guard H_BLORB_2 */

