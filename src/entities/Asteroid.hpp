#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include "Damageable.hpp"

/**
 * Asteroid object, split into smaller asteroids when destroyed
 */
class Asteroid: public Damageable {
public:
    enum Size {
        SMALL, MEDIUM, BIG
    };

    /**
     * @param size: size type
     * @param angle: movement direction
     */
    Asteroid(Size size=BIG, float angle=180);

    void onUpdate(float frametime) override;

    void onDestroy() override;

private:
    /**
     * Set a random image, according to the size
     */
    void setRandomImage();

    Size         m_size;
    sf::Vector2f m_speed;
    int          m_rotationSpeed;
};

#endif // ASTEROID_HPP

