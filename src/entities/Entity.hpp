#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

/**
 * Base de toute entité du jeu (classe abstraite)
 */
class Entity: public sf::Sprite
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
	 * Encaisser des dommages
	 */
	virtual void Hit(int damage);

	/**
	 * Déplacer l'entité
	 * @param[in] frametime: temps de la frame actuelle
	 */
	virtual void Move(float frametime) = 0;
	virtual void Action() {}

	//virtual void Update(float frametime) = 0;

	/**
	 * Notifier l'entité d'une collision
	 * @param[in, out] entity: entité entrée en collision
	 */
	virtual void OnCollide(Entity& entity);

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
		FlipX(it);
		flipped_ = it;
	}

	/**
	 * Obtenir la surface de collision du vaisseau
	 */
	virtual sf::FloatRect GetCollideRect() const;

	void SetID(int id);

	int GetID() const;

protected:
	int hp_;
	bool flipped_;

private:
	int id_;
};

#endif /* guard ENTITY_HPP */

