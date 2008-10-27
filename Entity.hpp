#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <map>
#include <utility>
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
    
    virtual void Action() = 0;
    
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
    
    enum Status {
                 BASE= 0,        // Entité non spawnée, non blocante
                 BLOCK= 0 << 1,  // Entité blocante
                 SPAWNED= 0 << 2 // Entité spawnée [pas morte]
                };
    
struct ltstatus
{
  bool operator()(const Status s1, const Status s2) const
  {
    return static_cast<int>(s1) < static_cast<int>(s2);
  }

};
    struct TimedEntity {
            int t;
            Entity* self;
    };
    
    typedef std::multimap<Entity::Status, TimedEntity, ltstatus > ManagedContainer;
    typedef std::multimap<Entity::Status, TimedEntity, ltstatus >::iterator ManagedIterator;
    typedef std::multimap<Entity::Status, TimedEntity, ltstatus >::const_iterator ManagedConstIterator;
protected:
    sf::Sprite sprite_;
    int hp_;
};

#endif /* guard ENTITY_HPP */

