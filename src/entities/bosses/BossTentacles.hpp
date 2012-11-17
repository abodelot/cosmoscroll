#ifndef BOSSTENTACLES_HPP
#define BOSSTENTACLES_HPP

#include "entities/Entity.hpp"
#include "entities/Animator.hpp"
#include "items/Weapon.hpp"


class BossTentacles: public Entity
{
public:
	BossTentacles(const sf::Vector2f& position);

	BossTentacles* Clone() const;

	// inherited
	void Update(float frametime);

	// inherited
	void OnDestroy();

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
	Animator m_animator;
};

#endif // BOSSTENTACLES_HPP
