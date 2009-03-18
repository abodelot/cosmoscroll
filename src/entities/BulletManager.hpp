#ifndef BULLETMANAGER_HPP
#define BULLETMANAGER_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "Entity.hpp"
#include "Weapon.hpp"

/**
 * Gestionnaire de projectiles (classe Singleton)
 */
class BulletManager
{
public:	
	/**
	 * Récupérer l'instance
	 */
	static BulletManager& GetInstance();
	
	/**
	 * Déplacer les projectiles
	 */
	void Update(float frametime);
	
	/**
	 * Ajouter un projectile dans le gestionnaire
	 */
	void Add(Weapon::Type type, Entity* sender, const sf::Vector2f& offset, float angle);
	
	/**
	 * Gestion des collisions
	 */
	void Collide(std::list<Entity*>& entities);
	
	/**
	 * Afficher les beams dans la fenêtre de rendu
	 */
	void Show(sf::RenderWindow& app);
	
	/**
	 * Suppression de tous les projectiles
	 */
	void Clear();
	
	/**
	 * Nettoyage des pointeurs sur Entity lors de la destruction de l'Entity
	 */
	void CleanSenders(Entity* target);
	
private:
	struct Bullet
	{
		sf::Sprite sprite;
		float angle;
		int damage;
		int speed;
		Entity* owner;
	};
	
	BulletManager();
	BulletManager(const BulletManager& other);
	BulletManager& operator=(const BulletManager& other);
	
	typedef std::list<Bullet>::iterator BulletIterator;
#ifndef NO_AUDIO
	sf::Sound sounds_[Weapon::WEAPON_COUNT];
#endif
	std::list<Bullet> bullets_;
};

#endif /* guard BULLETMANAGER_HPP */

