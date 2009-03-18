#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Entity.hpp"

#include <SFML/Graphics.hpp>

/**
 * Un astéroïde pouvant se diviser en petits morceaux
 */
class Asteroid: public Entity
{
public:
    enum Size
    {
        SMALL, MEDIUM, BIG
    };
    
    /**
     * Création d'un astéroïde
     * @param[in] offset: position
     * @param[in] size: taille
     * @param[in] angle: orientation
     */
    Asteroid(const sf::Vector2f& offset, Size size, float angle=180);
    
    void Move(float frametime);
    
    void Hit(int damage);
    
private:
    Size size_;
    float angle_;
    float speed_;
};

#endif /* guard ASTEROID_HPP */

