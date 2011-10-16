#ifndef PART_HPP
#define PART_HPP

#include "entities/Entity.hpp"

class ComplexEntity;

class Part: public Entity
{
public:
	Part(int id);

	// override
	void Update(float frametime);

	// override
	Entity* Clone() const;

	int GetID() const;

	void TakeDamage(int damage);

private:
	int id_;
	float timer_;
};


#endif // PART_HPP
