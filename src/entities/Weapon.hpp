#ifndef WEAPON_HPP
#define WEAPON_HPP

#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"


/**
 * Une arme associée à une entité
 */
class Weapon
{
public:
	Weapon();

	/**
	 * @param image: image du projectile
	 * @param fire_rate: nombre de tirs par seconde
	 * @param heat_cost: chaleur dégagée par tir
	 * @param damage: dégâts infligés par tir
	 * @param speed: vitesse du tir en pixels par seconde
	 */
	void Init(const sf::Image& image, float fire_rate, float heat_cost,
		int damage, int speed);

	/**
	 * @return true si l'arme peut être utilisée, sinon false
	 */
	bool IsInited() const;

	/**
	 * @param sound: son joué lors d'un tir (NULL si pas de son)
	 */
	void SetSoundName(const char* sound);

	/**
	 * Position de l'arme relative à la position de son propriétaire
	 */
	void SetOffset(int x, int y);

	void SetOwner(Entity* owner);

	/**
	 * Indique si l'arme est prête à tirer
	 */
	bool IsReady() const;

	/**
	 * @param offset: position du tireur
	 * @param angle: angle de tir en radians
	 */
	float Shoot(sf::Vector2f offset, float angle);

	void Update(float frametime);

	void SetMultiply(int n);

protected:
	virtual void ThrowHit(const sf::Vector2f& offset, float angle);

	Entity* GetOwner() const;

	const sf::Image* GetImage() const
	{
		return image_;
	}

	int GetSpeed() const
	{
		return speed_;
	}

	int GetDamage() const
	{
		return damage_;
	}

private:
	// cadence de tir
	float fire_rate_;
	float heat_cost_;
	int speed_;
	int damage_;
	const sf::Image* image_;

	// timer "compte à rebours" pour que la cadence de tir soit indépendante
	// du nombre de FPS. Si <= 0, on peut tirer. Si tir, timer reinitialisé.
	// le timer est mis à jour à chaque frame dans Update
	float fire_timer_;

	const char* sound_name_;
	int multiply_;
	Entity* owner_;

	bool inited_;
	int x_;
	int y_;
};

#endif // WEAPON_HPP
