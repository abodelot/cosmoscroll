#ifndef PLAYERSHIP_HPP
#define PLAYER_SHIP_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif

#ifndef JOYSTICK_ENABLED
#define JOYSTICK_ENABLED
#endif

#include "Entity.hpp"
#include "Weapon.hpp"
#include "ControlPanel.hpp"
#include "Bonus.hpp"
#include "Settings.hpp"

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
	void HandleJoyButton(const sf::Event::JoyButtonEvent& event);
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
	struct Config
	{
		sf::Key::Code up;
		sf::Key::Code down;
		sf::Key::Code left;
		sf::Key::Code right;
		sf::Key::Code weapon_a;
		sf::Key::Code weapon_b;
		sf::Key::Code bonus_cooler;


#ifdef JOYSTICK_ENABLED	
		unsigned int Jbonus_cooler;
		unsigned int Jweapon_a;
		unsigned int Jweapon_b;
		unsigned int Jvalid;
		unsigned int Jreturn;
#endif
	};
	

	
	enum TimedBonus
	{
		T_TRISHOT, T_SPEED, TIMED_BONUS_COUNT
	};
	
	void HandleBonus(const Bonus& bonus);
	
	void DisableTimedBonus(TimedBonus tbonus);
	
	// timers des bonus
	float bonus_[TIMED_BONUS_COUNT];
	
	ControlPanel& panel_;

	const sf::Input& input_;
	Config binds_;
	bool overheated_;
	float heat_;
	int coolers_;
	int shield_;
	float shield_timer_;
	int speed_;
#ifndef NO_AUDIO
	sf::Sound shield_sfx_;
#endif
	Weapon laserbeam_;
	Weapon hellfire_;
	
	Settings settings_;
};

#endif /* guard PLAYERSHIP_HPP */

