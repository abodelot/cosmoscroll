#ifndef ENNEMY_HPP
#define ENNEMY_HPP

#include "Entity.hpp"
#include "Weapon.hpp"

/*
 * Un vilain qui tire sur le joueur
 */
class Ennemy: public Entity
{
public:
    /*
     * @param[in] offset: position de départ
     * @param[in] target: vaisseau cible
     */
    Ennemy(const sf::Vector2f& offset, Entity** target);
    
    void Hit(int damage);
    
    /*
     * Déplacement
     */
    void Move(float frametime);
    
    void Action();
    
protected:
    Entity** target_;
    bool left_;
    Weapon weapon_;
};

#endif /* guard ENNEMY_HPP */

