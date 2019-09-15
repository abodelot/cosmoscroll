#ifndef EVILBOSS_HPP
#define EVILBOSS_HPP

#include "entities/Damageable.hpp"
#include "entities/Weapon.hpp"


class EvilBoss: public Damageable {
public:
    EvilBoss();

    void takeDamage(int damage) override;

    // callbacks ---------------------------------------------------------------

    void onInit() override;
    void onUpdate(float frametime) override;
    void onDestroy() override;

private:
    enum State {
        EVIL = 900, MORE_EVIL = 600, DAMN_EVIL = 300
    };

    State        m_state;
    State        m_next_state;
    sf::Vector2f m_speed;
    Weapon       m_eye_left;
    Weapon       m_eye_right;
    Entity*      m_target;
};

#endif // EVILBOSS_HPP
