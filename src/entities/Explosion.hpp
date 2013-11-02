#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "Entity.hpp"
#include "Animator.hpp"

/**
 * Dummy entity for displaying an explosion animation
 */
class Explosion: public Entity
{
public:
	Explosion();

	void collides(Entity& entity);
	Explosion* clone() const;

	void onUpdate(float frametime);

private:
	Animator  m_animator;
	sf::Clock m_clock;
};

#endif // EXPLOSION_HPP
