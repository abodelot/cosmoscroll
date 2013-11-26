#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Damageable.hpp"
#include "Animator.hpp"
#include "PowerUp.hpp"
#include "Missile.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "items/Weapon.hpp"

/**
 * Player's spaceship
 */
class Player: public Damageable
{
public:
	Player(const char* animation);

	~Player();

	void onEvent(const sf::Event& event);

	void HandleAction(Input::Action action);

	int getScore() const;

	void updateScore(int diff);

	// override
	void takeDamage(int damage);

	inline bool HasCheated() const { return m_konami_code_activated; }

	// callbacks ---------------------------------------------------------------

	void onInit();

	void onUpdate(float frametime);

	void onCollision(PowerUp& powerup);

	void onDestroy();

private:
	enum TimedPowerUp
	{
		T_DOUBLESHOT,
		T_TRISHOT,
		T_SPEED,
		TIMED_BONUS_COUNT
	};

	void Computemove(float frametime);

	/**
	 * Désactiver un bonus à effet temporaire
	 */
	void DisableTimedPowerUp(TimedPowerUp tbonus);

	/**
	 * Activer les effets du Code Konami
	 */
	void turnKonamiCodeOn();

	/**
	 * Set shield points
	 */
	void setShield(int shield);

	/**
	 * Faire beeper l'indicateur de surchauffe
	 */
	void AudibleHeatingCue();

	// Code Konami sequence
	static const int KONAMI_CODE_LENGTH = 10;
	Input::Action    m_konami_code[KONAMI_CODE_LENGTH];
	int              m_current_konami_index;
	bool             m_konami_code_activated;

	float bonus_[TIMED_BONUS_COUNT]; // timers des bonus
	bool overheated_;
	float heat_, heat_max_;
	float shield_timer_;
	int coolers_, missiles_;
	int shield_, shield_max_;
	int hp_max_;
	float speed_x_, speed_y_;
	int speed_max_;

	Input& input_;
	ControlPanel& panel_;
	Weapon<>        m_weapon;
	Weapon<Missile> m_missile_launcher;

	Animator m_animator;
	int      m_score;
};

#endif // PLAYER_HPP

