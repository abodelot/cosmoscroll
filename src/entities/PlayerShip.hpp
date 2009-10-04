#ifndef PLAYERSHIP_HPP
#define PLAYERSHIP_HPP

#include <SFML/System.hpp>

#include "Entity.hpp"
#include "Bonus.hpp"
#include "Weapon.hpp"
#include "MissileLauncher.hpp"
#include "../core/AbstractController.hpp"
#include "../core/ControlPanel.hpp"
#include "../core/Animated.hpp"

/**
 * Vaisseau spatial contrôlable par un joueur
 */
class PlayerShip: public Entity, public Animated
{
public:
	PlayerShip(const sf::Vector2f& position, const char* animation);

	~PlayerShip();

	// inherited
	PlayerShip* Clone() const;

	void HandleAction(AC::Action action);

	// inherited
	void OnCollide(Entity& entity);

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int damage);

	// inherited
	bool PixelPerfectCollide() const;

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
		T_TRISHOT,
		T_SPEED,
		T_STONED,
		TIMED_BONUS_COUNT
	};

	void ComputeMove(float frametime);

	void ComputeMoveOnDrugs(float frametime);

	/**
	 * Calculer la vitesse sur un axe
	 * @param speed: vitesse à mettre à jour
	 * @param lower: action diminuant la vitesse
	 * @param upper: action augmentant la vitesse
	 * @param diff: différence avec la nouvelle vitesse
	 */
	void ComputeAxisSpeed(float& speed, AC::Action lower, AC::Action upper, float diff);

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

	void IncreaseShield(int count = 1);

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
	int coolers_, shield_, missiles_;
	float speed_x_, speed_y_;
	float acceleration_delay_;
	int max_speed_;

	int controls_;

	AbstractController& controller_;
	ControlPanel& panel_;
	Weapon weapon1_;
	Weapon weapon2_;
	MissileLauncher missile_launcher_;

	void (PlayerShip::*compute_move_)(float);
};

#endif // PLAYERSHIP_HPP

