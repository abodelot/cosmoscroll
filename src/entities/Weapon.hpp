#ifndef WEAPON_HPP
#define WEAPON_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"


/**
 * Une arme associée à une entité
 */
class Weapon
{
public:
    /**
     * @param[in] image: image du projectile
     * @param[in] fire_rate: nombre de tirs par seconde
     * @param[in] heat_cost: chaleur dégagée par tir
     * @param[in] damage: dégâts infligés par tir
     * @param[in] speed: vitesse du tir en pixels par seconde
     * @param[in] sound: son joué lors d'un tir (NULL si pas de son)
     */
	Weapon(const sf::Image& image, float fire_rate, float heat_cost, int damage, int speed, const char* sound = NULL);

	void SetOffset(int x, int y);

	void SetOwner(Entity* owner);

    float Shoot(sf::Vector2f offset, float angle = 0.f);

    void Update(float frametime);

    void SetTriple(bool triple);

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


	const char* sound_;
    bool triple_;
	Entity* owner_;

	int x_;
	int y_;
};

#endif // WEAPON_HPP
