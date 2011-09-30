#ifndef BOSSTENTACLES_HPP
#define BOSSTENTACLES_HPP

#include "Entity.hpp"
#include "Animated.hpp"
#include "items/Weapon.hpp"


class BossTentacles: public Entity, public Animated
{
public:
	BossTentacles(const sf::Vector2f& position);

	BossTentacles* Clone() const;

	// inherited
	void Update(float frametime);

	// inherited
	void TakeDamage(int damage);

	// inherited
	bool PixelPerfectCollide() const;

	// inherited
	void SetTarget(Entity* target);

private:
	enum Move
	{
		INIT,
		LURK,
		IDLE,
		CHARGE
	};

	Move move_;

	float timer_;
	int speed_x_;
	int speed_y_;
	Weapon weapon_;
	Entity* target_;
};

#endif // BOSSTENTACLES_HPP
