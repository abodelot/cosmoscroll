#ifndef PART_HPP
#define PART_HPP

#include "entities/Entity.hpp"

class ComplexEntity;

class Part: public Entity
{
public:
	Part(int id=-1);

	// override
	void onUpdate(float frametime);

	// override
	Entity* clone() const;

	// override
	void onCollision(const Entity& entity);

	int GetID() const;

	void setDestructible(bool destructible);

	void setParent(ComplexEntity* parent);

	void onDestroy();

private:
	int  m_id;
	bool m_destructible;
	ComplexEntity* m_parent;
};


#endif // PART_HPP
