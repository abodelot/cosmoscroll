#ifndef ENTITY_HPP
#define ENTITY_HPP

/*#include <map>
#include <utility>*/
#include <SFML/Graphics.hpp>

/*
TODO: (re)faire une classe intermédiare SpaceShip pour mieux factoriser le code
de Shoot (fire_timer, fire_rate...), Asteroid n'en a pas besoin :o)
Et gérer les sorties de l'écran dans la méthode Move (mettre heath à 0)
*/


/*
 * Classe abstraite représentant une entité du jeu
 */
class Entity
{
public:
    /*
     * @param[in] img: image du vaisseau
     * @param[in] offset: position de départ
     * @param[in] hp: points de vie
     */
    Entity(const sf::Image& img, const sf::Vector2f& offset, int hp=3);

    virtual ~Entity();
        
    /*
     * Afficher le vaisseau
     */
    virtual void Show(sf::RenderWindow& app) const;
    
    /*
     * Encaisser des dommages
     */
    virtual void Hit(int damage);
    
    virtual void Move(float frametime) = 0;
    
    // fusionner avec Move et Action dans une méthode Update ??
    virtual void Action();
    
    /*
     * Comportement de l'entité si ent entre en collision avec elle
     */
    virtual void Collide(const Entity& ent);
    
    bool IsDead();
    
    void Kill();
    
    /*
     * Obtenir la position du vaisseau
     */
    virtual sf::Vector2f GetPosition();
    
    /*
     * Obtenir la surface de collision du vaisseau
     */
    virtual sf::FloatRect GetRect();
    
protected:
    sf::Sprite sprite_;
    int hp_;
};

#endif /* guard ENTITY_HPP */

