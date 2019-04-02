#ifndef GATE_HPP
#define GATE_HPP

#include "entities/MultiPartEntity.hpp"
#include "entities/Animator.hpp"

class Gate: public MultiPartEntity
{
public:
    Gate();

    void onUpdate(float frametime);

private:
    void onPartDestroyed(const Part& part) override;

    int m_energy_cells_count;
    float m_door_timer;
    float m_door_height;
    Animator m_cell_animator1;
    Animator m_cell_animator2;
};


#endif // GATE_HPP
