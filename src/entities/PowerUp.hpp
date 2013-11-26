#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"


class PowerUp: public Entity
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
		SUPER_BANANA, //
		POWERUP_COUNT
	};

	PowerUp(Type type);

	void collides(Entity& entity);

	/**
	 * Drop a random powerup at a given position
	 */
	static void dropRandom(const sf::Vector2f& position);

	/**
	 * Powerup's description text
	 */
	const std::wstring& getDescription() const;

	Type getType() const;

	static sf::IntRect getTextureRect(Type type);

	// callbacks ---------------------------------------------------------------

	void onUpdate(float frametime);

private:
	Type m_type;
};

#endif // POWERUP_HPP

