#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Weapon.hpp"
#include "Animation.hpp"
#include "core/ParticleSystem.hpp"

class LevelManager;
class Entity;
class Spaceship;
class Player;


/**
 * Gestionnaire de la vie, l'univers et tout le reste
 */
class EntityManager: public sf::Drawable, public sf::Transformable
{
public:
	// background image speed for parallax scrolling
	static const int BACKGROUND_SPEED = 20;
	static const int FOREGROUND_SPEED = 80;

	enum Mode
	{
		MODE_STORY,
		MODE_ARCADE
	};

	/**
	 * @return instance unique
	 */
	static EntityManager& getInstance();

	/**
	 * Initialiser un mode de jeu avant une partie
	 * Initialize a game mode before a game
	 */
	void InitMode(Mode mode);

	Mode GetMode() const;

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
	 * Number of managed entites
	 */
	size_t count() const;

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

	inline float GetTimer() const { return timer_; }

	void createImpactParticles(const sf::Vector2f& pos, size_t count);
	void createGreenParticles(const sf::Vector2f& pos, size_t count);

private:
	EntityManager();
	EntityManager(const EntityManager&);
	~EntityManager();

	Entity* createRandomEntity();
	void RegisterUniqueEntity(Spaceship* entity);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * Spawn randomly next entities, using Arcade Mode
	 * @return true if there are entities left
	 */
	bool arcadeModeCallback();

	/**
	 * Spawn next entities in the level, using Story Mode
	 * @return true if there are entities left
	 */
	bool storyModeCallback();

	/**
	 * Re-allocate player
	 */
	void RespawnPlayer();

	typedef std::map<std::string, Animation> AnimationMap;
	AnimationMap m_animations;

	typedef std::map<std::string, Weapon> WeaponMap;
	WeaponMap m_weapons;

	typedef std::map<std::string, Spaceship*> SpaceshipMap;
	SpaceshipMap m_spaceships;

	typedef std::list<Entity*> EntityList;
	EntityList m_entities;

	std::vector<Spaceship*> uniques_;

	int m_width;
	int m_height;
	int decor_height_;

	Mode mode_;

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

	Player* m_player;
	bool (EntityManager::*more_bad_guys_)();
	float timer_;
	LevelManager& m_levels;

	int max_droppable_index_;
	int max_droppable_points_;

	class ParallaxLayer: public sf::Drawable
	{
	public:
		friend class EntityManager;

		ParallaxLayer();
		void scroll(float frametime);
		void setTexture(const sf::Texture& texture);
		void setColor(const sf::Color& color);

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		float         m_speed;
		sf::Sprite    m_background;
		sf::Sprite    m_background2;
		sf::BlendMode m_blend_mode;
	};

	ParallaxLayer layer1_;
	ParallaxLayer layer2_;
};

#endif // ENTITYMANAGER_HPP
