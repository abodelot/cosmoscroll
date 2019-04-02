#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include "Damageable.hpp"
#include "Animator.hpp"
#include "entities/Weapon.hpp"

class Spaceship: public Damageable
{
public:
    enum MovementPattern
    {
        MAGNET, LINE, SINUS, CIRCLE
    };

    enum AttackPattern
    {
        AUTO_AIM, ON_SIGHT, NONE
    };

    Spaceship(const Animation& animation, int hp, int speed);
    ~Spaceship();

    void setMovementPattern(MovementPattern movement);

    void setAttackPattern(AttackPattern attack);

    Spaceship* clone() const;

    Weapon& getWeapon();

    void setPoints(int points);
    int getPoints() const;

    // callbacks ---------------------------------------------------------------

    void onInit() override;
    void onUpdate(float frametime) override;
    void onDestroy() override;

private:
    AttackPattern   m_attack;
    MovementPattern m_movement;

    int       m_points;
    Weapon    m_weapon;
    Entity*   m_target;
    Animator  m_animator;

    int       m_speed;
    float     m_origin_y;
    float     m_angle;
};

#endif // SPACESHIP_HPP
