#ifndef GATE_HPP
#define GATE_HPP

#include "MultiPartEntity.hpp"


class Gate: public MultiPartEntity
{
public:
	Gate();

	void onUpdate(float frametime);

	Entity* clone() const;
private:
	// override
	void onPartDestroyed(const Part& part);

	int energy_cells_;
	float door_timer_;
	float door_full_height_;
};


#endif // GATE_HPP
