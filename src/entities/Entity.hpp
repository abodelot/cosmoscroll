#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <SFML/Graphics.hpp>

class Damageable;
class PowerUp;
class Projectile;

/**
 * Abstract base class for game objects
 */
class Entity: public sf::Sprite
{
public:
    enum Team
    {
        GOOD, NEUTRAL, BAD
    };

    Entity();

    /**
     * If true, entity will be removed from the scene
     */
    bool isDead() const;

    /**
     * Destroy entity and trigger onDestroy callback
     */
    void kill();

    Team getTeam() const;

    // implement to trigger collision callbacks
    virtual void collides(Entity& entity) = 0;

    /**
     * Register texture for pixel-perfect collision when attached to sprite
     */
    void setTexture(const sf::Texture& texture);


    // callbacks ---------------------------------------------------------------

    /**
     * Called when entity is inserted in the entity manager
     */
    virtual void onInit() {}

    /**
     * Called each frame
     */
    virtual void onUpdate(float frametime) = 0;

    /**
     * Called when entity is killed during the game
     */
    virtual void onDestroy() {}

    virtual void onCollision(Entity&)     {}
    virtual void onCollision(Damageable&) {}
    virtual void onCollision(PowerUp&)    {}
    virtual void onCollision(Projectile&) {}

    // helpers -----------------------------------------------------------------

    sf::FloatRect getBoundingBox() const;

    inline float getWidth() const  { return getTextureRect().width; }
    inline float getHeight() const { return getTextureRect().height; }

    inline float getX() const { return getPosition().x; }
    inline float getY() const { return getPosition().y; }
    inline void setX(float x) { setPosition(x, getPosition().y); }
    inline void setY(float y) { setPosition(getPosition().x, y); }

    sf::Vector2f getCenter() const;

    // ugly hacks --------------------------------------------------------------

    virtual float getSpeedX() const { return 0.f; }
    virtual float getSpeedY() const { return 0.f; }

protected:
    void setTeam(Team team);

private:
    bool m_dead;
    Team m_team;
};

#endif // ENTITY_HPP
