#ifndef DAMAGEABLE_HPP
#define DAMAGEABLE_HPP

#include "Entity.hpp"

/**
 *
 */
class Damageable: public Entity
{
public:
	Damageable();

	virtual void collides(Entity& entity);

	virtual void takeDamage(int damage);

	int getHP() const;

	// callbacks ---------------------------------------------------------------

	void onCollision(Damageable& entity);
	void onCollision(Projectile& projectile);
	void onDestroy();

protected:
	void setHP(int hp);
	int updateHP(int diff);

	void updateDamageFlash(float frametime);

private:
	int   m_hp;
	float m_flash_timer;
};

#endif // DAMAGEABLE_HPP

