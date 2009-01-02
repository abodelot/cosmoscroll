#ifndef WEAPON_HPP
#define WEAPON_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif
#include <SFML/System/Vector2.hpp>

#include "Entity.hpp"

class BulletManager;

/**
 * Une arme associée à une entité
 */
class Weapon
{
public:
    enum Type
    {
        LASERBEAM, HELLFIRE, PLASMACANNON, DEVILSEYES, WEAPON_COUNT
    };
    
    /**
     * @param[in] type: type de l'arme
     * @parma[in] sender: entité qui possède l'arme
     */
	Weapon(Type type, Entity* sender);
    
    ~Weapon();
    
    float Shoot(const sf::Vector2f& offset, float angle = 0.f);
    
    void Update(float frametime);
    
    void SetTriple(bool triple);
    
private:
    float energy_cost_;
    // timer "compte à rebours" pour que la cadence de tir soit indépendante
    // du nombre de FPS. Si <= 0, on peut tirer. Si tir, timer reinitialisé.
    // le timer est mis à jour à chaque frame dans Update
    float fire_timer_;
    // cadence de tir
    float fire_rate_;
    Type type_;
    
    bool triple_;
	Entity* owner_;
	static BulletManager& bullets_;
};

#endif /* guard WEAPON_HPP */

