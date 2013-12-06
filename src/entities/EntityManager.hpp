#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Animation.hpp"

class ParticleSystem;
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
	static const int BACKGROUND_SPEED = 15;
	static const int FOREGROUND_SPEED = 60;

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
	 * Dimensions de l'univers
	 */
	void resize(int width, int height);

	/**
	 * @return largeur de l'univers
	 */
	inline int getWidth() const {return m_width;};

	/**
	 * @return hauteur de l'univers
	 */
	inline int getHeight() const{return m_height;};

	/**
	 * Mettre à jour les entités
	 * @param frametime: temps de la frame courante
	 */
	void Update(float frametime);

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
	 * Mettre fin à la partie en cours
	 */
	void TerminateGame();

	/**
	 * Tester si le jeu est terminé
	 * @return true si game over
	 */
	bool IsGameOver();

	/**
	 * Allocate a new Spaceship with a given profile
	 * @param id: spaceship-profile identifier
	 * @return new spaceship
	 */
	Spaceship* createSpaceship(const std::string& id) const;

	/**
	 * Obtenir une animation
	 * @param key: identifiant de l'animation
	 */
	const Animation& getAnimation(const std::string& key) const;

	/**
	 * Charger les définitions XML des animations
	 * @param filename: fichier XML des animations
	 */
	void loadAnimations(const std::string& filename);

	/**
	 * Charger les défintions XML des vaisseaux
	 * @param filename: fichier XML des vaisseaux
	 */
	void loadSpaceships(const std::string& filename);

	/**
	 * Obtenir le vaisseau du joueur
	 */
	Player* GetPlayerShip() const;

	inline float GetTimer() const { return timer_; }

private:
	EntityManager();
	EntityManager(const EntityManager&);
	~EntityManager();

	Entity* createRandomEntity();
	void RegisterUniqueEntity(Spaceship* entity);

	// inherited
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Spawner les prochains éléments du jeu en mode Arcade
	 * @return true si jeu terminé
	 */
	bool MoreBadGuys_ARCADE();

	/**
	 * Spawner les prochains éléments du jeu en mode Story
	 * @return true si jeu terminé
	 */
	bool MoreBadGuys_STORY();

	/**
	 * Ré-allouer le vaisseau du joueur
	 */
	void RespawnPlayer();

	typedef std::map<std::string, Spaceship*> SpaceshipMap;
	SpaceshipMap m_spaceships;

	typedef std::map<std::string, Animation> AnimationMap;
	AnimationMap animations_;

	typedef std::list<Entity*> EntityList;
	EntityList m_entities;

	std::vector<Spaceship*> uniques_;

	int m_width;
	int m_height;
	int decor_height_;

	Mode mode_;

	ParticleSystem& particles_;

	Player* m_player;
	bool (EntityManager::*more_bad_guys_)();
	bool game_over_;
	float timer_;
	LevelManager& levels_;

	int max_droppable_index_;
	int max_droppable_points_;

	struct ParallaxLayer: public sf::Drawable
	{
		float scrolling_speed_;
		const sf::Texture* m_texture;
		sf::Sprite background_;
		sf::Sprite background2_;
		sf::BlendMode blend_mode_;

		ParallaxLayer();
		void OnUpdate(float frametime);
		void SetScrollingTexture(const sf::Texture* image);
		void setColor(const sf::Color& color);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	};

	ParallaxLayer layer1_;
	ParallaxLayer layer2_;
};

#endif // ENTITYMANAGER_HPP
