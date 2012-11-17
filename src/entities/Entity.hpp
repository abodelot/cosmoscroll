#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

enum CollisionBehavior
{
	C_IGNORE_DAMAGE = 1 << 0,
	C_IGNORE_HITS   = 1 << 1
};

/**
 * Abstrct base class for game objects
 */
class Entity: public sf::Sprite
{
public:
	enum Team
	{
		GOOD, NEUTRAL, BAD
	};

	/**
     * @param position: position at spawing
	 * @param hp: Hit Points
	 * @param collide_damage: inflicted damages in a collision
	 */
	Entity(const sf::Vector2f& position, int hp, int collide_damage = 1);

	virtual ~Entity();

	virtual Entity* Clone() const = 0;

	/**
	 * @return sprite center
	 */
	sf::Vector2f getCenter() const;

	// override
	void setTexture(const sf::Texture& texture);

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
	inline bool IsDead() const { return hp_ <= 0; }

	/**
	 * Kill entity (will be removed from entity manager)
	 */
	void Kill();

	/**
	 * @return entity remaining health points
	 */
	int GetHP() const;
	void SetHP(int hp);

	virtual float GetSpeedX() const { return 0.f; }
	virtual float GetSpeedY() const { return 0.f; };

	/**
	 * Obtenir la surface de collision du vaisseau
	 */
	sf::FloatRect GetCollideRect() const;

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

	inline float getWidth() const { return getTextureRect().width; }
	inline float getHeight() const { return getTextureRect().height; }

	inline void setX(float x) { setPosition(x, getPosition().y); }
	inline void setY(float y) { setPosition(getPosition().x, y); }

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
	int hp_;
	int points_;
	int collide_damage_;
	int collide_flag_;
	Team team_;
	float flash_timer_;
};

#endif // ENTITY_HPP

