#ifndef PART_HPP
#define PART_HPP

#include "entities/Damageable.hpp"

class MultiPartEntity;

class Part: public Damageable
{
public:
	Part(int id = -1, int hp = 1, Team team = NEUTRAL);

	void onUpdate(float frametime) override;

	void takeDamage(int damage) override;

	int getID() const;

	void setDestructible(bool destructible);

	void setParent(MultiPartEntity* parent);

	void onDestroy() override;

private:
	int  m_id;
	bool m_destructible;
	MultiPartEntity* m_parent;
};


#endif // PART_HPP
