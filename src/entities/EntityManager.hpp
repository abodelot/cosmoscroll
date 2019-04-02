#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Weapon.hpp"
#include "Animation.hpp"
#include "Spaceship.hpp"
#include "core/ParticleSystem.hpp"

class LevelManager;
class Entity;
class Player;


/**
 * Singleton factory and manager for entities
 */
class EntityManager: public sf::Drawable, public sf::Transformable
{
public:
    // background image speed for parallax scrolling
    static const int BACKGROUND_SPEED = 20;
    static const int FOREGROUND_SPEED = 80;

    enum Mode
    {
        LEVELS_MODE,
        INFINITY_MODE
    };

    /**
     * @return instance unique
     */
    static EntityManager& getInstance();

    /**
     * Initialiser un mode de jeu avant une partie
     * Initialize a game mode before a game
     */
    void setMode(Mode mode);

    Mode getMode() const;

    /**
     * Resize the universe dimensions
     */
    void resize(int width, int height);

    /**
     * @return universe width
     */
    inline int getWidth() const {return m_width;};

    /**
     * @return universe height
     */
    inline int getHeight() const{return m_height;};

    /**
     * Spawn next enemies
     * @return true if game over
     */
    bool spawnBadGuys();

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
     * Load animation definitions
     * @param filename: path to XML document
     */
    void loadAnimations(const std::string& filename);

    /**
     * Get an animation
     * @param id: animation ID from XML document
     */
    const Animation& getAnimation(const std::string& id) const;

    /**
     * Load weapon definitions
     * @param filename path to XML document
     */
    void loadWeapons(const std::string& filename);

    /**
     * Get a weapon
     * @param id: weapon ID from XML document
     */
    const Weapon& getWeapon(const std::string& id) const;

    /**
     * Load spaceship definitions
     * @param filename: path to XML document
     */
    void loadSpaceships(const std::string& filename);

    /**
     * Allocate a new Spaceship with a given profile
     * @param id: spaceship type ID from XML document
     * @return new allocated spaceship
     */
    Spaceship* createSpaceship(const std::string& id) const;

    /**
     * Get player entity
     */
    Player* getPlayer() const;

    inline float getTimer() const { return m_timer; }

    void createImpactParticles(const sf::Vector2f& pos, size_t count);
    void createGreenParticles(const sf::Vector2f& pos, size_t count);

private:
    EntityManager();
    EntityManager(const EntityManager&);
    ~EntityManager();

    Entity* createRandomEntity();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    /**
     * Randomly spawn next entities in INFINITY_MODE
     * @return true if there are entities left
     */
    bool infinityModeCallback();

    /**
     * Spawn next entities in the level in LEVELS_MODE
     * @return true if there are entities left
     */
    bool levelsCallback();

    /**
     * Re-allocate player
     */
    void respawnPlayer();

    typedef std::list<Entity*> EntityList;
    EntityList m_entities;

    typedef std::map<std::string, Animation> AnimationMap;
    AnimationMap m_animations;

    typedef std::map<std::string, Weapon> WeaponMap;
    WeaponMap m_weapons;

    typedef std::map<std::string, Spaceship> SpaceshipMap;
    SpaceshipMap m_spaceships; // ID-indexed spaceships (Levels mode)

    std::vector<Spaceship> m_sorted_ships; // Random access (Infinity mode)

    bool (EntityManager::*m_spawnMethod)();

    Mode          m_mode;
    float         m_timer;
    Player*       m_player;
    int           m_width;
    int           m_height;
    int           m_decor_height;
    LevelManager& m_levels;
    int           m_max_droppable_index;
    int           m_max_droppable_points;

    // Particles ---------------------------------------------------------------
    class StarsEmitter: public ParticleSystem::Emitter
    {
    protected:
        void onParticleUpdated(ParticleSystem::Particle& particle, float) const override;

        void onParticleCreated(ParticleSystem::Particle& particle) const override;
    };

    ParticleSystem&         m_particles;
    ParticleSystem::Emitter m_impact_emitter;
    ParticleSystem::Emitter m_green_emitter;
    StarsEmitter            m_stars_emitter;

    // Parallax scrolling ------------------------------------------------------
    class ParallaxLayer: public sf::Drawable
    {
    public:
        ParallaxLayer();

        void resetScrolling();
        void scroll(float delta);
        void setTexture(const sf::Texture& texture);
        void setColor(const sf::Color& color);

    private:
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        const sf::Texture* m_texture;
        sf::Vertex         m_vertices[8];
        sf::BlendMode      m_blend_mode;
    };

    ParallaxLayer m_layer1;
    ParallaxLayer m_layer2;
};

#endif // ENTITYMANAGER_HPP
