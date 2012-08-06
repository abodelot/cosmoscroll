#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

class Entity;


/**
 * Une arme associée à une entité
 */
class Weapon
{
public:
	Weapon();

	/**
	 * Initialiser une arme
	 * @param weapon_id: identifiant de l'arme (cf. weapons.xml)
	 */
	void Init(const char* weapon_id);

	/**
	 * @return true si l'arme peut être utilisée, sinon false
	 */
	bool IsInited() const;

	/**
	 * Utiliser l'arme
	 * @param angle: angle de tir en radians
	 * @param pos: position cible (trajectoire auto-calculée)
	 */
	float Shoot(float angle);
	float ShootAt(const sf::Vector2f& pos);

	void Update(float frametime);

	/**
	 * @param image: image du projectile
	 */
	void setTexture(const sf::Texture* image);

	/**
	 * @param fire_rate: nombre de tirs par seconde
	 */
	void SetFireRate(float shot_per_sec);

	/**
	 * @param heat_cost: chaleur dégagée par tir
	 */
	void SetHeatCost(float heat_cost);

	/**
	 * @param damage: dégâts infligés par tir
	 */
	void SetDamage(int damage);

	/**
	 * @param velocity: vitesse du tir en pixels par seconde
	 */
	void SetVelociy(int velocity);

	/**
	 * @param sound: son joué lors d'un tir (NULL si pas de son)
	 */
	void SetSound(const sf::SoundBuffer* sound);

	/**
	 * Position de l'arme relative à la position de son propriétaire
	 */
	void SetOffset(int x, int y);
	void SetOffset(const sf::Vector2f& offset);

	/**
	 * @param owner: entité propriétaire de l'arme
	 */
	void SetOwner(Entity* owner);

	/**
	 * Indique si l'arme est prête à tirer
	 */
	bool IsReady() const;



	void SetMultiply(int n);

protected:
	virtual void ThrowHit(const sf::Vector2f& offset, float angle);

	Entity* GetOwner() const;

	const sf::Texture* getTexture() const
	{
		return image_;
	}

	int GetVelocity() const
	{
		return velocity_;
	}

	int GetDamage() const
	{
		return damage_;
	}

private:
	// cadence de tir
	float fire_rate_;
	float heat_cost_;
	int velocity_;
	int damage_;
	const sf::Texture* image_;

	// timer "compte à rebours" pour que la cadence de tir soit indépendante
	// du nombre de FPS. Si <= 0, on peut tirer. Si tir, timer reinitialisé.
	// le timer est mis à jour à chaque frame dans Update
	float fire_timer_;

	const sf::SoundBuffer* sound_;
	int multiply_;
	Entity* owner_;

	bool inited_;
	int x_;
	int y_;
};

#endif // WEAPON_HPP
