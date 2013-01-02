#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Asteroid;
class ComplexEntity;
class Spaceship;
class Player;
class PowerUp;
class Projectile;


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
	 */
	Entity(const sf::Vector2f& position, int hp);

	virtual ~Entity();

	virtual Entity* clone() const = 0;

	/**
	 * Called when entity is inserted in the entity manager
	 */
	virtual void onInit() {}

	/**
	 * @return sprite center
	 */
	sf::Vector2f getCenter() const;

	// override
	void setTexture(const sf::Texture& texture);

	/**
	 * Encaisser des dommages
	 */
	virtual void takeDamage(int damage);

	/**
	 * Mettre à jour l'entité
	 * @param frametime: temps de la frame actuelle
	 */
	virtual void onUpdate(float frametime) = 0;

	virtual void onDestroy() {}

	/**
	 * Collision callback
	 */
	virtual void onCollision(const Entity& entity);

	// cast
	virtual const PowerUp* toPowerUp() const   { return NULL; }
	virtual const ComplexEntity*    toComplexEntity() const { return NULL; }
	virtual const Player*  toPlayer() const    { return NULL; }
	virtual const Projectile* toProjectile() const { return NULL; }

	/**
	 * Détermine si l'entité est encore en vie
	 * @return true si l'entité doit être supprimée
	 */
	inline bool isDead() const { return m_hp <= 0; }

	/**
	 * Kill entity (will be removed from entity manager)
	 */
	void kill();

	/**
	 * @return entity remaining health points
	 */
	int getHP() const;
	void setHP(int hp);

	virtual float GetSpeedX() const { return 0.f; }
	virtual float GetSpeedY() const { return 0.f; };

	/**
	 * Obtenir la surface de collision du vaisseau
	 */
	sf::FloatRect GetCollideRect() const;

	/**
	 * Obtenir l'équipe de l'entité
	 */
	Team getTeam() const;

	/**
	 * Valeur de l'entité
	 */
	void setPoints(int points);
	int getPoints() const;

	inline float getWidth() const { return getTextureRect().width; }
	inline float getHeight() const { return getTextureRect().height; }

	inline float getX() const { return getPosition().x; }
	inline float getY() const { return getPosition().y; }
	inline void setX(float x) { setPosition(x, getPosition().y); }
	inline void setY(float y) { setPosition(getPosition().x, y); }

protected:
	/**
	 * Attribuer une équipe à l'entité (défaut: NEUTRAL)
	 */
	void setTeam(Team team);

	int UpdateHP(int diff);

	void UpdateFlash(float frametime);
private:
	int m_hp;
	int m_points;
	Team m_team;
	float flash_timer_;
};

#endif // ENTITY_HPP

