#ifndef H_ENTITY
#define H_ENTITY

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
     */
    Entity(const sf::Image& img, const sf::Vector2f& offset);

    virtual ~Entity();
        
    /*
     * Afficher le vaisseau
     */
    virtual void Show(sf::RenderWindow& app);
    
    /*
     * Encaisser des dommages
     * @return true si le vaisseau est détruit, sinon false
     */
    virtual void Hit(int damage);
    
    virtual void Move(float frametime) = 0;
    
    virtual void Action() = 0;
    
    virtual bool IsDead();
    
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
    int health_;
};

#endif /* guard H_ENTITY */

