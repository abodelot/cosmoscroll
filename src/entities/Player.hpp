#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Damageable.hpp"
#include "Animator.hpp"
#include "PowerUp.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "entities/Weapon.hpp"

/**
 * Player's spaceship
 */
class Player: public Damageable
{
public:
	Player();
	~Player();

	void onActionDown(Action::ID action);

	void onActionUp(Action::ID action);

	void onEvent(const sf::Event& event);

	int getScore() const;

	void updateScore(int diff);

	void takeDamage(int damage) override;

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

	/**
	 * Unapply a powerup when duration is elapsed
	 */
	void DisableTimedPowerUp(TimedPowerUp tbonus);

	/**
	 * Apply some upgrades when konami code sequence is activated
	 */
	void applyKonamiCode();

	/**
	 * Set shield points
	 */
	void setShield(int shield);

	/**
	 * Warn the player if heat has reached a given threshold by playing an audio signal
	 */
	void overheatAudioHint() const;

	ControlPanel& panel_;

	// Code Konami sequence
	static const int KONAMI_CODE_LENGTH = 10;
	Action::ID       m_konami_code[KONAMI_CODE_LENGTH];
	int              m_current_konami_index;
	bool             m_konami_code_activated;

	float bonus_[TIMED_BONUS_COUNT]; // timers des bonus
	bool overheated_;
	float heat_, heat_max_;
	float shield_timer_;
	int shield_, shield_max_;
	int hp_max_;
	float m_speed;
	int   m_missiles;
	int   m_icecubes;

	Weapon           m_weapon;
	Weapon           m_missile_launcher;

	Animator         m_animator;
	const Animation& m_animation_up;
	const Animation& m_animation_down;
	const Animation& m_animation_normal;

	int              m_score;

	class ShieldEmitter: public ParticleSystem::Emitter
	{
	public:
		void createParticles(size_t count);

		void onParticleUpdated(ParticleSystem::Particle& particle, float frametime) const override;
	};

	ShieldEmitter           m_shield_emitter;
	ParticleSystem::Emitter m_smoke_emitter;
	ParticleSystem::Emitter m_snowflakes_emitter;
	ParticleSystem::Emitter m_powerup_emitter;
};

#endif // PLAYER_HPP

