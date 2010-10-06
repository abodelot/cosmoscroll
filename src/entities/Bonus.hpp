#ifndef BONUS_HPP
#define BONUS_HPP

#include "Entity.hpp"


class Bonus: public Entity
{
public:
	enum Type
	{
		HEALTH,       // +1 hp
		SHIELD,       // +1 shield
		COOLER,       // +1 cooler
		MISSILE,      // +1 cooler
		TRIPLE_SHOT,  // shot * 3 (10s)
		SPEED,        // speed * 2 (10s)
		STONED,       // inertia (10s)
		SUPER_BANANA, // invincible (10s)
		BONUS_COUNT
	};

	Bonus(Type type, const sf::Vector2f& offset);

	Bonus* Clone() const;

	// allouer un bonus aléatoirement
	static Bonus* MakeRandom(const sf::Vector2f& offset);

	// inherited
	void TakeDamage(int damage);

	// inherited
	void Update(float frametime);

	/**
	 * Description textuelle du bonus
	 */
	const wchar_t* GetDescription() const;

	Type GetType() const;

	static sf::IntRect GetSubRect(Type type);

private:
	Type type_;
};

#endif // BONUS_HPP

