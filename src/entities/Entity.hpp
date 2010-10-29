#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

/**
 * Base de toute entité du jeu (classe abstraite)
 */
class Entity: public sf::Sprite
{
public:
	enum Team
	{
		GOOD, NEUTRAL, BAD
	};

	/**
	 * @param position: position (x, y) de départ
	 * @param hp: points de vie
	 * @param collide_damage: dégâts infligés lors d'une collision
	 */
	Entity(const sf::Vector2f& position, int hp, int collide_damage = 1);

	virtual ~Entity();

	virtual Entity* Clone() const = 0;

	virtual void SetTarget(Entity* target);

	/**
	 * Encaisser des dommages
	 */
	virtual void TakeDamage(int damage);

	/**
	 * Mettre à jour l'entité
	 * @param frametime: temps de la frame actuelle
	 */
	virtual void Update(float frametime) = 0;

	/**
	 * Notifier l'entité d'une collision
	 * @param entity: entité entrée en collision
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
	 * @return entity remaining health points
	 */
	int GetHP() const;

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
	sf::FloatRect GetCollideRect() const;
	virtual void GetCollideRect(sf::FloatRect& rect) const;

	/**
	 * Indique si l'entité utilise une détection de collision pixel perfect
	 */
	virtual bool PixelPerfectCollide() const;

	/**
	 * Indique si l'entité est en collision avec une autre entité
	 */
	bool IsCollidingWith(const Entity& other);

	/**
	 * Obtenir l'équipe de l'entité
	 */
	Team GetTeam() const;

	int GetCollideDamage() const;

protected:
	/**
	 * Attribuer une équipe à l'entité (défaut: NEUTRAL)
	 */
	void SetTeam(Team team);

	int UpdateHP(int diff);
	void SetHP(int hp);



private:
	bool flipped_;
	int hp_;
	int collide_damage_;
	Team team_;
};

#endif // ENTITY_HPP

