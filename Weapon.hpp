#ifndef H_WEAPON
#define H_WEAPON

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif
#include <SFML/System/Vector2.hpp>


class Weapon
{
public:
    enum Type
    {
        LASERBEAM, HELLFIRE, PLASMACANNON, WEAPON_COUNT
    };
    
    Weapon(Type type);
    
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
};

#endif /* guard H_WEAPON */

