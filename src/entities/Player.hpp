#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Entity.hpp"
#include "Animator.hpp"
#include "PowerUp.hpp"
#include "Missile.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"
#include "items/Weapon.hpp"

/**
 * Vaisseau spatial contrôlable par un joueur
 */
class Player: public Entity
{
public:
	Player(const sf::Vector2f& position, const char* animation);

	~Player();

	void Initialize();

	/**
	 * @return points de bouclier
	 */
	inline int GetShield() const { return shield_; }

	inline bool HasCheated() const { return konami_code_activated_; }

	void UpdateScoreCounter(int diff);

	// override
	Player* clone() const;

	void HandleAction(Input::Action action);

	// override
	void onUpdate(float frametime);

	// override
	void onCollision(const Entity& entity);

	// override
	void onDestroy();

	// override
	void takeDamage(int damage);

	const Player* toPlayer() const { return this; }

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
	 * Gérer un bonus attrapé
	 */
	void HandlePowerUp(PowerUp::Type bonus_t);

	/**
	 * Désactiver un bonus à effet temporaire
	 */
	void DisableTimedPowerUp(TimedPowerUp tbonus);

	/**
	 * Activer les effets du Code Konami
	 */
	void KonamiCodeOn();

	/**
	 * Augmenter l'énergie du bouclier
	 * @param count: quantité à ajouter
	 */
	void IncreaseShield(int count = 1);

	/**
	 * Faire beeper l'indicateur de surchauffe
	 */
	void AudibleHeatingCue();

	enum
	{
		KONAMI_CODE_LENGTH = 10
	};

	// la séquence du Code Konami
	Input::Action konami_code_[KONAMI_CODE_LENGTH];
	int current_konami_event_;
	bool konami_code_activated_;

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
};

#endif // PLAYER_HPP

