#ifndef FLYINGSAUCERBOSS_HPP
#define FLYINGSAUCERBOSS_HPP

#include "entities/Damageable.hpp"
#include "entities/Weapon.hpp"


class FlyingSaucerBoss: public Damageable
{
public:
    FlyingSaucerBoss();

    void onUpdate(float frametime) override;

    void takeDamage(int damage) override;

    void onInit() override;

private:
    Entity* m_target;
    float m_timer;
    float m_angle;
    Weapon m_left_tube;
    Weapon m_right_tube;
};


#endif // FLYINGSAUCERBOSS_HPP
