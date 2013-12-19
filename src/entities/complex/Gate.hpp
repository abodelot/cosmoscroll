#ifndef GATE_HPP
#define GATE_HPP

#include "MultiPartEntity.hpp"


class Gate: public MultiPartEntity
{
public:
	Gate();

	void onUpdate(float frametime);

private:
	void onPartDestroyed(const Part& part) override;

	int energy_cells_;
	float door_timer_;
	float door_full_height_;
};


#endif // GATE_HPP
