#ifndef H_BLORB
#define H_BLORB

#include "Entity.hpp"
#include "Weapon.hpp"

#include <SFML/Graphics.hpp>

/*
 * Un machin qui tire des laserbeams sur le player
 */
class Blorb: public Entity
{
public:
    /*
     * @param[in] offset: position de départ
     * @param[in] target: vaisseau cible
     */
    Blorb(const sf::Vector2f& offset, Entity** target);
    
    void Hit(int damage);
    
    /*
     * Déplacement
     */
    void Move(float frametime);
    
    void Action();
    
protected:
    Entity** target_;
    bool old_orient_;
    Weapon weapon_;
};

#endif /* guard H_BLORB */

