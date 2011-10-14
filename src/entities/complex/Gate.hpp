#ifndef GATE_HPP
#define GATE_HPP

#include "ComplexEntity.hpp"


class Gate: public ComplexEntity
{
public:
	Gate(const sf::Vector2f& pos);

	void Update(float frametime);

	Entity* Clone() const;
private:
	// override
	void OnPartDestroyed(const Part& part);

	int energy_cells_;
	float door_timer_;
	float door_full_height_;

};


#endif // GATE_HPP
