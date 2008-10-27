#ifndef PLAYERSHIP_HPP
#define PLAYER_SHIP_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif

#include "Entity.hpp"
#include "Weapon.hpp"
#include "ControlPanel.hpp"

/*
 * Le vaisseau aux couleurs de starsky et hutch
 */
class PlayerShip: public Entity
{
public:
    PlayerShip(const sf::Vector2f& offset, const sf::Input& input);
    
    // phase de tir
    void Action();
    
    /*
     * Déplacer le vaisseau
     */
    void Move(float frametime);
    
    void Hit(int damage);

private:
    ControlPanel& panel_;
    const sf::Input& input_;
    bool is_lighten_;
    
    bool overheated_;
    float heat_;
    
    int shield_;
    float shield_timer_;
#ifndef NO_AUDIO
    sf::Sound shield_sfx_;
#endif
    
    Weapon laserbeam_;
    Weapon hellfire_;
};

#endif /* guard PLAYERSHIP_HPP */

