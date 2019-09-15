#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <string>
#include <list>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>

#include "core/Tileset.hpp"
#include "core/Tilemap.hpp"
#include "core/ParallaxLayer.hpp"
#include "particles/ParticleSystem.hpp"
#include "particles/ParticleEmitter.hpp"

class Entity;
class Player;

/**
 * Singleton factory and manager for entities
 */
class EntityManager: public sf::Drawable {
public:
    /**
     * @return instance unique
     */
    static EntityManager& getInstance();

    void initializeLevel();

    /**
     * Resize the universe dimensions
     */
    void resize(int width, int height);

    /**
     * @return universe size, in pixels
     */
    inline const sf::Vector2i& getSize() const { return m_size; };

    /**
     * Spawn next enemies
     * @return true if game over
     */
    bool gameOverOrCompleted() const;

    /**
     * Update managed entities and resolve collisions
     */
    void update(float frametime);

    /**
     * Insert an entity in the scene
     */
    void addEntity(Entity* entity);

    /**
     * Delete all managed entities
     */
    void clearEntities();

    /**
     * Get player entity
     */
    Player* getPlayer() const;

    void scheduleForSpawning(Entity* entity);

    inline float getTimer() const { return m_timer; }

    /**
     * Get rectangle area currently displayed by the view
     */
    sf::FloatRect getViewZone() const;

    void createImpactParticles(const sf::Vector2f& pos, size_t count);
    void createGreenParticles(const sf::Vector2f& pos, size_t count);

    void toggleFastScrolling();

private:
    EntityManager();
    EntityManager(const EntityManager&);
    ~EntityManager();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void spawnEntities();

    /**
     * Re-allocate player
     */
    void respawnPlayer();

    void clearSpawnQueue();

    bool collidesWithTilemap(const sf::FloatRect& rect, Entity& entity) const;

    typedef std::list<Entity*> EntityList;
    EntityList m_entities;

    sf::View      m_view;
    float         m_timer;
    float         m_traveled;
    float         m_scrollingSpeed;
    float         m_stopScrollingAt;
    Player*       m_player;
    sf::Vector2i  m_size;

    // Entity are sorted by X position in the spawn queue, so we don't need to loop on the entire
    // queue when looking up for the next entity to spawn
    struct PosComparator {
        bool operator()(const Entity* a, const Entity* b) const;
    };
    std::priority_queue<Entity*, std::vector<Entity*>, PosComparator> m_spawnQueue;

    // Particles ---------------------------------------------------------------
    class StarsEmitter: public ParticleEmitter {
        using ParticleEmitter::ParticleEmitter;

    protected:
        void onParticleUpdated(Particle& particle, float) const override;

        void onParticleCreated(Particle& particle) const override;
    };

    ParticleSystem& m_particleSystem;
    ParticleEmitter m_impactEmitter;
    ParticleEmitter m_greenEmitter;
    StarsEmitter    m_starsEmitter;

    std::vector<ParallaxLayer> m_layers;

    Tileset m_tileset;
    Tilemap m_tilemap;

    mutable sf::RectangleShape m_collision;
    mutable sf::RectangleShape m_collisionPoint;
    mutable sf::RectangleShape m_allPoints[8];
    mutable sf::RectangleShape m_allCheckTiles[8];
};

#endif
