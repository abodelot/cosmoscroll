#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

enum CollisionBehavior
{
	C_IGNORE_DAMAGE = 1 << 0,
	C_IGNORE_HITS   = 1 << 1
};

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

	/**
	 * @return sprite center
	 */
	sf::Vector2f GetCenter_() const;

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

	virtual void OnDestroy() {};

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
	void SetHP(int hp);


	void FlipX(bool flip);
	void FlipY(bool flip);


	inline bool IsFlippedX() const { return flipped_x_; }
	inline bool IsFlippedY() const { return flipped_y_; }


	virtual float GetSpeedX() const { return 0.f; }
	virtual float GetSpeedY() const { return 0.f; };

	/**
	 * Obtenir la surface de collision du vaisseau
	 */
	sf::FloatRect GetCollideRect() const;

	/**
	 * Test if entity is colliding with another entity
	 * @param other: other colliding entity
	 * @param r1: pre-computed entity collide rect
	 * @param r2: pre-computed other entity collide rec
	 */
	bool IsCollidingWith(const Entity& other, const sf::FloatRect& r1, const sf::FloatRect& r2);

	/**
	 * Obtenir l'équipe de l'entité
	 */
	Team GetTeam() const;

	int GetCollideDamage() const;


	void SetCollideFlag(int collision_flag);
	int GetCollideFlag() const;


	/**
	 * Valeur de l'entité
	 */
	void SetPoints(int points);
	int GetPoints() const;
	int ConsumePoints();

protected:
	/**
	 * Attribuer une équipe à l'entité (défaut: NEUTRAL)
	 */
	void SetTeam(Team team);

	int UpdateHP(int diff);

	/**
	 * Set inflicted damage on collision
	 */
	void SetCollideDamage(int damage);

	void UpdateFlash(float frametime);
private:
	bool flipped_x_;
	bool flipped_y_;
	int hp_;
	int points_;
	int collide_damage_;
	int collide_flag_;
	Team team_;
	float flash_timer_;
};

#endif // ENTITY_HPP

