#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "Entity.hpp"
#include "Weapon.hpp"


/**
 * Il est très méchant, et il en veut !
 */
class EvilBoss: public Entity
{
public:
	EvilBoss(const sf::Vector2f& position);

	EvilBoss* Clone() const;

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int damage);

	// inherited
	bool PixelPerfectCollide() const;
	
	// inherited
	void SetTarget(Entity* target);
	
private:
	enum Phase
	{
		EVIL = 600, MORE_EVIL = 400, DAMN_EVIL = 200
	};

	Phase phase_, next_;

	bool left_;
	Weapon eye_left_;
	Weapon eye_right_;
	Weapon canon_;
	Entity* target_;
};

#endif // EVILBOSS_HPP
