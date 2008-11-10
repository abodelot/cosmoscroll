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
    
	
	inline bool EffectsPaused() const
	{
		return pause_effects_;
	}
		
	void PauseEffects(bool b)
	{
		GetMutex().Lock();
		pause_effects_ = b;
		GetMutex().Unlock();
	}
    
	inline int GetShield() const
	{
		return shield_;
	}
	
	inline int GetCoolers() const
	{
		return coolers_;
	}
	
	inline void SetShield(int s)
	{
		shield_ = s;
	}
	
	inline void SetCoolers(int c)
	{
		coolers_ = c;
	}

	void Neutralize();
	
private:
	struct Config
	{
		sf::Key::Code up;
		sf::Key::Code down;
		sf::Key::Code left;
		sf::Key::Code right;
		sf::Key::Code weapon_a;
		sf::Key::Code weapon_b;
		sf::Key::Code bonus_cooler;
	};

	void HandleBonus(const Bonus& bonus);

	static void EndBonusWrapper(void* data);				

	void EndBonus();
	
	inline sf::Thread& GetThread()
	{
		static sf::Thread thread_(PlayerShip::EndBonusWrapper, this);
		return thread_;
	}
	
	inline static sf::Mutex& GetMutex()
	{
		static sf::Mutex mutex_;
		return mutex_;
	}
	
	inline int GetTimer()
	{
		GetMutex().Lock();
		int i = trigun_timer_;
		GetMutex().Unlock();
		return i;
	}
	
	inline void SetTimer(int t)
	{
		GetMutex().Lock();
		trigun_timer_ = t;
		GetMutex().Unlock();
	}


	
	int trigun_timer_;
   	int coolers_;	
    ControlPanel& panel_;
    const sf::Input& input_;
    Config binds_;
    bool pause_effects_;
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

