#ifndef DAMAGEABLE_HPP
#define DAMAGEABLE_HPP

#include "Entity.hpp"

/**
 * Base class for damageable entities with hit points
 */
class Damageable: public Entity {
public:
    Damageable();

    virtual void collides(Entity& entity) override;

    virtual void takeDamage(int damage);

    int getHP() const;

    // callbacks ---------------------------------------------------------------

    void onCollision(Damageable& entity) override;
    void onCollision(Projectile& projectile) override;
    void onTileCollision() override;
    void onDestroy() override;

    void initDamageFlash();

protected:
    void setHP(int hp);
    int updateHP(int diff);

    void updateDamageFlash(float frametime);

private:
    int   m_hp;
    float m_flash_timer;
};

#endif // DAMAGEABLE_HPP
