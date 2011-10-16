#ifndef PLAYERSHIP_HPP
#define PLAYERSHIP_HPP

#include <SFML/Audio.hpp>

#include "Entity.hpp"
#include "Animated.hpp"
#include "Bonus.hpp"
#include "items/Weapon.hpp"
#include "items/MissileLauncher.hpp"
#include "core/Input.hpp"
#include "core/ControlPanel.hpp"

/**
 * Vaisseau spatial contrôlable par un joueur
 */
class PlayerShip: public Entity, public Animated
{
public:
	PlayerShip(const sf::Vector2f& position, const char* animation);

	~PlayerShip();

	void Initialize();

	/**
	 * @return points de bouclier
	 */
	inline int GetShield() const { return shield_; }

	inline bool HasCheated() const { return konami_code_activated_; }

	void UpdateScoreCounter(int diff);

	// override
	PlayerShip* Clone() const;

	void HandleAction(Input::Action action);

	// override
	void OnCollide(Entity& entity);

	// override
	void Update(float frametime);

	// override
	void TakeDamage(int damage);

private:
	enum TimedBonus
	{
		T_DOUBLESHOT,
		T_TRISHOT,
		T_SPEED,
		TIMED_BONUS_COUNT
	};

	void ComputeMove(float frametime);

	/**
	 * Gérer un bonus attrapé
	 */
	void HandleBonus(Bonus::Type bonus_t);

	/**
	 * Désactiver un bonus à effet temporaire
	 */
	void DisableTimedBonus(TimedBonus tbonus);

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
	 * @param frametime
	 */
	void AudibleHeatingCue(float frametime);

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
	Weapon weapon1_;
	Weapon weapon2_;
	MissileLauncher missile_launcher_;
	const sf::SoundBuffer* snd_disabled_, *snd_overheat_;
};

#endif // PLAYERSHIP_HPP

