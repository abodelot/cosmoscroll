#ifndef PLAYERSHIP_HPP
#define PLAYERSHIP_HPP

#ifndef NO_AUDIO
#include <SFML/Audio.hpp>
#endif
#include <SFML/System.hpp>

#include "../core/AbstractController.hpp"
#include "Bonus.hpp"
#include "../core/ControlPanel.hpp"
#include "Entity.hpp"
#include "Weapon.hpp"
#include "../core/Animated.hpp"

/**
 * Vaisseau spatial contrôlable par un joueur
 */
class PlayerShip: public Entity, public Animated
{
public:
	PlayerShip(const sf::Vector2f& offset, const char* image);
	
	~PlayerShip();
	
	void HandleAction(AC::Action action);
	
	void Action();
	
	void Collide(Entity& ent);
	
	/*
	 * Déplacer le vaisseau
	 */
	void Move(float frametime);
	
	void Hit(int damage);
	
	inline void SetControls(int controls)
	{
		controls_ = controls;
	}
	
	inline int GetControls() const
	{
		return controls_;
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
	int coolers_, shield_, speed_;
	
	int controls_;
#ifndef NO_AUDIO
	sf::Sound shield_sfx_;
#endif
	AbstractController& controller_;
	ControlPanel& panel_;
	Weapon laserbeam_, hellfire_;
};

#endif /* guard PLAYERSHIP_HPP */

