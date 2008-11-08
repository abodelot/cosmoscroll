#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

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
    virtual void Collide(Entity& ent);
    
    bool IsDead();
    
    void Kill();
    
    void KillIfOut();
    
	inline int GetHP() {
		return hp_;};
	
	inline void SetHP(int val) {
		hp_ = val;};
  /*
     * Obtenir la position du vaisseau
     */
    virtual sf::Vector2f GetPosition();
    
    /*
     * Obtenir la surface de collision du vaisseau
     */
    virtual sf::FloatRect GetRect();
	
	
	inline void SetPosition(sf::Vector2f pos) {
		sprite_.SetPosition(pos);};
    
protected:
    sf::Sprite sprite_;
    int hp_;
};

#endif /* guard ENTITY_HPP */

