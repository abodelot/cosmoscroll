#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <string>
#include <list>
#include <map>
#include <vector>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Weapon.hpp"
#include "SpaceShip.hpp"

/**
 * Gestionnaire de la vie, l'univers et tout le reste
 */
class EntityManager: public sf::Drawable
{
public:
	/**
	 * @return l'instance unique
	 */
	static EntityManager& GetInstance();

	/**
	 * Dimensions de l'univers
	 */
	void SetSize(int width, int height);

	/**
	 * @return largeur de l'univers
	 */
	int GetWidth() const;

	/**
	 * @return hauteur de l'univers
	 */
	int GetHeight() const;

	/**
	 * Mettre à jour les entités
	 * @param frametime: temps de la frame courante
	 */
	void Update(float frametime);

	/**
	 * Ajouter une entité
	 * @param entity: entité à ajouter
	 */
	void AddEntity(Entity* entity);

	/**
	 * Supprimer toutes les entités
	 */
	void Clear();

	/**
	 * @return nombre d'entités
	 */
	int Count() const;

	/**
	 * Initialiser une arme
	 * @param id: type de l'arme demandé
	 * @param weapon: arme à initialiser
	 */
	void InitWeapon(int id, Weapon* weapon) const;

	/**
	 * Allouer un vaisseau
	 * @param id: type du vaisseau demandé
	 * @param x: position x (pixels)
	 * @param y: position y (pixels)
	 * @return vaisseau
	 */
	SpaceShip* CreateSpaceShip(int id, int x, int y);

	/**
	 * Obtenir une animation
	 * @param key: identifiant de l'animation
	 */
	const Animation& GetAnimation(const char* key) const;

	/**
	 * Charger les définitions des XML armes
	 * @param filename: fichier XML des armes
	 */
	void LoadWeapons(const char* filename);

	/**
	 * Charger les définitions XML des animations
	 * @param filename: fichier XML des animations
	 */
	void LoadAnimations(const char* filename);

	/**
	 * Charger les défintions XML des vaisseaux
	 * @param filename: fichier XML des vaisseaux
	 */
	void LoadSpaceShips(const char* filename);

	Entity* CreateRandomEntity() const;

	/**
	 * Appliquer un foncteur sur chaque entité
	 * @param functor: foncteur prenant une entité en paramètre
	 */
	template <class T>
	void ApplyToEach(T& functor);

private:
	EntityManager();
	EntityManager(const EntityManager&);
	~EntityManager();

	// inherited
	void Render(sf::RenderTarget& target) const;

	struct WeaponDefinition
	{
		std::string name;       // generic name
		float heat_cost;        // cost per shot
		float fire_rate;        // shot per second
		int speed;              // bullet speed (pixels per second)
		int damage;             // inflicted damage
		const sf::Image* image; // hit image
		std::string sound;      // sound name
	};


	typedef std::map<int, SpaceShip*> SpaceShipMap;
	SpaceShipMap spaceships_defs_;

	typedef std::map<std::string, Animation> AnimationMap;
	AnimationMap animations_;

	typedef std::map<int, WeaponDefinition> WeaponMap;
	WeaponMap weapon_defs_;

	typedef std::list<Entity*> EntityList;
	EntityList entities_;

	std::vector<Entity*> uniques_;

	int width_;
	int height_;
};


template <class T>
void EntityManager::ApplyToEach(T& functor)
{
	for (EntityList::iterator it = entities_.begin();
		it != entities_.end(); ++it)
	{
		functor(**it);
	}
}

#endif // ENTITYMANAGER_HPP
