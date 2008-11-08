#ifndef PLAYERSHIP_HPP
#define PLAYER_SHIP_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif

#include "Entity.hpp"
#include "Weapon.hpp"
#include "ControlPanel.hpp"
#include "Bonus.hpp"

#include <SFML/System.hpp>

/*
 * Le vaisseau aux couleurs de starsky et hutch
 */
class PlayerShip: public Entity
{
public:
    PlayerShip(const sf::Vector2f& offset, const sf::Input& input);
    
    ~PlayerShip();
    
    void HandleKey(const sf::Event::KeyEvent& key);
    
    // phase de tir
    void Action();
    
    void Collide(Entity& ent);
    /*
     * Déplacer le vaisseau
     */
    void Move(float frametime);
    
    void Hit(int damage);
    
    // je doc plus tard
    
    static void EndBonusWrapper(void* data);

    void EndBonus();

    void HandleBonus(const Bonus& bonus);
	
	inline bool EffectsPaused() {
		return pause_effects_;};
		
	inline void PauseEffects(bool b) {
		pause_effects_ = b;};
    
		
	inline int getShield() {
		return shield_;};
	
	inline int getCoolers() {
		return coolers_;};
		
	inline void setShield(int s) {
		shield_ = s;};

	inline void  setCoolers(int c) {
		coolers_ = c;};

private:

	inline sf::Thread& GetTrigunThread() {
		static sf::Thread thread_(PlayerShip::EndBonusWrapper, this);
		return thread_;
	};

    ControlPanel& panel_;
    const sf::Input& input_;
    
    bool overheated_;
    float heat_;
    
	int coolers_;
	
    int shield_;
    float shield_timer_;
#ifndef NO_AUDIO
    sf::Sound shield_sfx_;
#endif
/*
le thread sert a désactiver le bonus au bout de n secondes
on aurait aussi pu faire sans (un if dans le Update pour savoir s'il y a bonus)
-> je teste :)
*/
    //sf::Thread thread_;
    int trigun_timer_;
    
    Weapon laserbeam_;
    Weapon hellfire_;
	bool pause_effects_;
};

#endif /* guard PLAYERSHIP_HPP */

