#ifndef POWERUP_HPP
#define POWERUP_HPP

#include "Entity.hpp"

/**
 * Passive entity, can be picked up by the player
 */
class PowerUp: public Entity
{
public:
	enum Type
	{
		REPAIR,       // +1 hp
		FULL_REPAIR,  // Restore all hp
		SHIELD,       // +1 shield
		FULL_SHIELD,  // Restore all shield
		ICECUBE,      // +1 icecube
		MISSILE,      // +1 missile
		DOUBLE_SHOT,  // Double bullets (10s)
		TRIPLE_SHOT,  // Triple bullets (10s)
		SPEED,        // Increase speed (10s)
		_COUNT        // INTERNAL USE
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

