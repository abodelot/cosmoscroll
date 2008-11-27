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

	PlayerShip(const sf::Vector2f& offset);
	
	~PlayerShip();
	
	void HandleAction(AC::Action action);
/*	void HandleKey(const sf::Event::KeyEvent& key);
	void HandleJoyButton(const sf::Event::JoyButtonEvent& event);*/
	// phase de tir
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

private:

	enum TimedBonus
	{
		T_TRISHOT, T_SPEED, TIMED_BONUS_COUNT
	};

	void HandleBonus(const Bonus& bonus);

	void DisableTimedBonus(TimedBonus tbonus);

	float bonus_[TIMED_BONUS_COUNT]; // timers des bonus

	bool overheated_;
	float heat_, shield_timer_;
	int coolers_, shield_, speed_;

#ifndef NO_AUDIO
	sf::Sound shield_sfx_;
#endif

	AbstractController& controls_;
	ControlPanel& panel_;
	Weapon laserbeam_, hellfire_;

};

#endif /* guard PLAYERSHIP_HPP */
