#ifndef ENNEMY_HPP
#define ENNEMY_HPP

#include "Entity.hpp"
#include "Weapon.hpp"

/*
 * Classe abstraite
 */
class Ennemy: public Entity
{
public:
    /*
     * @param[in] offset: position de départ
     * @param[in] target: vaisseau cible
     */
    Ennemy(const sf::Vector2f& offset, const sf::Image& img, int hp,
        Entity* target);
    
    void Hit(int damage);
    
    /*
     * Déplacement
     */
    virtual void Move(float frametime) = 0;
    
    enum Type
    {
        BLORB, INTERCEPTOR, DRONE
    };
    
    static Ennemy* Make(Type type, const sf::Vector2f& offset, Entity* target);

protected:
    Entity* target_;
};

#endif /* guard ENNEMY_HPP */

