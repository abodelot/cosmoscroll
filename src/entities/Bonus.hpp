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
		DOUBLE_SHOT,  // shot * 2 (10s)
		TRIPLE_SHOT,  // shot * 3 (10s)
		SPEED,        // speed * 2 (10s)
		SUPER_BANANA, // invincible (10s)
		BONUS_COUNT
	};

	Bonus(Type type, const sf::Vector2f& offset);

	Bonus* Clone() const;

	// allouer un bonus aléatoirement
	static Bonus* MakeRandom(const sf::Vector2f& offset);

	// inherited
	void Update(float frametime);

	/**
	 * Description textuelle du bonus
	 */
	const std::wstring& GetDescription() const;

	Type GetType() const;

	static sf::IntRect GetSubRect(Type type);

private:
	Type type_;
};

#endif // BONUS_HPP

