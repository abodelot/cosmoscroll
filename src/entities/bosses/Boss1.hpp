#ifndef BOSS_1_HPP
#define BOSS_1_HPP

#include "entities/Damageable.hpp"
#include "entities/Weapon.hpp"

class Boss1: public Damageable {
public:
    Boss1();

    void onUpdate(float frametime) override;

private:
    enum State { IDLE, CIRCLE, MISSILE, ATTACK };

    float m_angle;
    sf::Vector2f m_circleCenter;
    float m_timer;
    State m_state;
    Weapon m_weapon;
    Weapon m_missile;
};

#endif
