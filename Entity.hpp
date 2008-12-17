#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

/**
 * Base de toute entité du jeu (classe abstraite)
 */
class Entity
{
public:
	/**
	 * @param[in] img: image du vaisseau
	 * @param[in] offset: position de départ
	 * @param[in] hp: points de vie
	 */
	Entity(const sf::Image& img, const sf::Vector2f& offset, int hp=3);
	
	virtual ~Entity();
	
	/**
	 * Afficher le vaisseau
	 */
	virtual void Show(sf::RenderWindow& app) const;
	
	/**
	 * Encaisser des dommages
	 */
	virtual void Hit(int damage);
	
	/**
	 * Déplacer l'entité
	 * @param[in] frametime: temps de la frame actuelle
	 */
	virtual void Move(float frametime) = 0;
	virtual void Action() {}	

	/**
	 * Comportement de l'entité si ent entre en collision avec elle
	 */
	virtual void Collide(Entity& ent);
	
	/**
	 * Détermine si l'entité est encore en vie
	 * @return true si l'entité doit être supprimée
	 */
	bool IsDead() const
	{
		return hp_ <= 0;
	}
	
	/**
	 * Tuer l'entité
	 */
	void Kill()
	{
		hp_ = 0;
	}
	
	/**
	 * Supprime l'unité si elle est hors de l'univers
	 */
	void KillIfOut();
	
	virtual inline bool IsFlipped()
	{
		return flipped_;
	}
	
	virtual inline void Flip(bool it)
	{
		sprite_.FlipX(it);
		flipped_ = it;
	}
	
	/**
	 * Obtenir la position du vaisseau
	 */
	virtual sf::Vector2f GetPosition() const
	{
		return sprite_.GetPosition();
	}

	inline void SetPosition(const sf::Vector2f& offset)
	{
		sprite_.SetPosition(offset);
	}
	
	/**
	 * Obtenir la surface de collision du vaisseau
	 */
	virtual sf::FloatRect GetRect() const;

	
	/*virtual inline void UseLimits(bool it)
	{
		use_limits_ = it;
	}

	virtual inline bool UsesLimits()
	{
		return use_limits_;
	}
	
	virtual inline void SetLimits(sf::Vector2f& x, sf::Vector2f& y)
	{
		x_limits_ = x;
		y_limits_ = y;
	}*/
	
protected:
	sf::Sprite sprite_;
	int hp_;
	bool flipped_;
	
	/*bool use_limits_;
	sf::Vector2f x_limits_, y_limits_;*/
};

#endif /* guard ENTITY_HPP */

