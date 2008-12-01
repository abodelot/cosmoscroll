#ifndef PLAYERSHIP_HPP
#define PLAYER_SHIP_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif
#include <SFML/System.hpp>

#include "AbstractController.hpp"
#include "Bonus.hpp"
#include "ControlPanel.hpp"
#include "Entity.hpp"
#include "Weapon.hpp"


/*
 * Le vaisseau aux couleurs de starsky et hutch
 */
class PlayerShip: public Entity
{
public:
	PlayerShip(const sf::Vector2f& offset, const char* image="spaceship-red");
	
	~PlayerShip();
	
	void HandleAction(AC::Action action);
	
	void Action();
	
	void Collide(Entity& ent);
	/*
	 * Déplacer le vaisseau
	 */
	void Move(float frametime);
	
	void Hit(int damage);
	
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

	inline void SetControls(int id)
	{
		my_controls_ = id;
	}
	
	inline void NoShot()
	{
		overheated_ = true;
	}

	
private:

	enum TimedBonus
	{
		T_TRISHOT, T_SPEED, TIMED_BONUS_COUNT
	};
	
	/**
	 * Gérer un bonus attrapé
	 */
	void HandleBonus(const Bonus& bonus);
	
	/**
	 * Désactiver un bonus à effet temporaire
	 */
	void DisableTimedBonus(TimedBonus tbonus);
	
	/**
	 * Activer les effets du Code Konami
	 */
	void KonamiCodeOn();
	
	enum
	{
		KONAMI_CODE_LENGTH = 10
	};
	
	// la séquence du Code Konami
	AC::Action konami_code_[KONAMI_CODE_LENGTH]; 
	int current_konami_event_;
	
	float bonus_[TIMED_BONUS_COUNT]; // timers des bonus

	bool overheated_;
	float heat_, shield_timer_;
	int coolers_, shield_, speed_, my_controls_;
	
#ifndef NO_AUDIO
	sf::Sound shield_sfx_;
#endif

	AbstractController& controls_;
	ControlPanel& panel_;
	Weapon laserbeam_, hellfire_;
	

};

#endif /* guard PLAYERSHIP_HPP */
