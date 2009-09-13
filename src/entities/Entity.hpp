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
	 * @param[in] collide_damage: dégâts infligés lors d'une collision
	 */
	Entity(const sf::Image& img, const sf::Vector2f& offset, int hp,
		int collide_damage = 1);

	virtual ~Entity();

	/**
	 * Encaisser des dommages
	 */
	virtual void TakeDamage(int damage);

	/**
	 * Mettre à jour l'entité
	 * @param[in] frametime: temps de la frame actuelle
	 */
	virtual void Update(float frametime) = 0;

	/**
	 * Notifier l'entité d'une collision
	 * @param[in, out] entity: entité entrée en collision
	 */
	virtual void OnCollide(Entity& entity);

	/**
	 * Détermine si l'entité est encore en vie
	 * @return true si l'entité doit être supprimée
	 */
	inline bool IsDead() const
	{
		return hp_ <= 0;
	}

	/**
	 * Tuer l'entité
	 */
	void Kill();

	/**
	 * Supprime l'unité si elle est hors de l'univers
	 */
	void KillIfOut();

	inline bool IsFlipped() const
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
	virtual void GetCollideRect(sf::FloatRect& rect) const;

	/**
	 * Attribuer un identifiant à l'entité
	 */
	void SetID(int id);

	/**
	 * Obtenir l'identifiant de l'entité
	 */
	int GetID() const;

protected:
	int hp_;
	bool flipped_;

private:
	int collide_damage_;
	int id_;
};

#endif // ENTITY_HPP

