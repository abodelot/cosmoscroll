#ifndef FLYINGSAUCERBOSS_HPP
#define FLYINGSAUCERBOSS_HPP

#include "entities/Damageable.hpp"
#include "entities/Weapon.hpp"
#include <SFML/System/Vector2.hpp>

class FlyingSaucerBoss: public Damageable {
public:
    FlyingSaucerBoss();

    void setSpawnPosition(const sf::Vector2f& position) override;

    void onUpdate(float frametime) override;

    void takeDamage(int damage) override;

    void onInit() override;

private:
    Entity* m_target;
    float m_timer;
    float m_angle;
    Weapon m_leftTube;
    Weapon m_rightTube;
    sf::Vector2f m_spawnPosition;
};

#endif
