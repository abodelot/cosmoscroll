#ifndef H_ASTEROID
#define H_ASTEROID

#include "Entity.hpp"

#include <SFML/Graphics.hpp>

class Asteroid: public Entity
{
public:
    enum Size
    {
        SMALL, MEDIUM, BIG
    };
    
    /*
     * Création d'un astéroïde
     * @param[in] offset: position
     * @param[in] size: taille
     * @param[in] game: référence sur le gestionnaire d'entités
     */
    Asteroid(const sf::Vector2f& offset, Size size);
    
    void Action();
    
    void Move(float frametime);
    
    void Hit(int damage);
    
private:
    Size size_;
    float angle_;
    float speed_;
};

#endif /* guard H_ASTEROID */

