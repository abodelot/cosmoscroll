#ifndef BONUS_HPP
#define BONUS_HPP

#include "Entity.hpp"

/*
 * Donne +1 HP
 * (attention l'effet est codé dans Entity::Collid, trouver un moyen
 * d'obtenir les effets du bonus depuis Entity::Collid (un/des getter sus Bonus,
 * un truc du genre...)
 */
class Bonus: public Entity
{
public:
    Bonus(const sf::Vector2f& offset);
    
    void Hit(int damage);
    
    void Move(float frametime);
};

#endif /* guard BONUS_HPP */

