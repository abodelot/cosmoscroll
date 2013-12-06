#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <queue>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "utils/tinyxml/tinyxml2.h"

class Entity;

/**
 * Singleton du gestionnaire de niveaux (file d'attente des unités à venir)
 */
class LevelManager
{
public:
	/**
	 * @return instance
	 */
	static LevelManager& getInstance();

	/**
	 * Parse the XML file storing the levels definitions
	 */
	void loadLevels(const std::string& file);

	void loadCurrent();

	/**
	 * Get the next entity to be spawned in the current level
	 * @param timer: elapsed time (seconds) in the level
	 * @return entity if any, or NULL
	 */
	Entity* GiveNextEntity(float timer);

	/**
	 * Size of the waiting line in the current level
	 * @return number of remaining entities
	 */
	inline int RemainingEntities() const
	{
		return waiting_line_.size();
	};

	/**
	 * Set the current level
	 * @param level_num: level number in [1 - level_max]
	 */
	void setCurrent(size_t level_num);
	size_t getCurrent() const;


	/**
	 * Last reached level
	 */
	size_t unlockNextLevel();
	void setLastUnlocked(size_t level);
	size_t getLastUnlocked() const;

	// attributs du niveau courant

	/**
	 * Obtenir la description d'un niveau
	 * @return chaîne de description
	 */
	const char* GetDescription() const;

	/**
	 * Get scrolling background image used in current level
	 * @return image, or NULL if none
	 */
	const sf::Texture* GetLayerImage1() const;
	const sf::Texture* GetLayerImage2() const;
	sf::Color GetLayerColor() const;

	int GetDecorHeight() const;
	int GetStarsCount() const;

	const char* GetMusic() const;

	/**
	 * Durée du niveau en secondes
	 */
	int GetDuration() const;

	/**
	 * Obtenir le nombre de niveaux chargés
	 */
	size_t getLevelCount() const;

	/**
	 * @return true si tous les niveaux sont terminés
	 */
	bool AllLevelsCompleted() const;

	/**
	 * Total des points disponibles dans le niveau courant
	 * @return points
	 */
	int GetTotalPoints() const;

private:
	LevelManager();
	LevelManager(const LevelManager&);
	~LevelManager();

	/**
	 * Parser un niveau
	 * @param elem: noeud XML contenant le niveau
	 */
	void ParseLevel(tinyxml2::XMLElement* elem);

	/**
	 * Parser un élément de niveau
	 */
	void ParseEntity(tinyxml2::XMLElement* elem);

	/**
	 * Ajouter une entité à la file d'attente
	 * @param entity: entité à ajouter en fin de queue
	 * @param time: temps de l'apparition relatif au temps de la dernière entité
	 */
	void AppendToWaitingLine(Entity* entity, float time);

	/**
	 * Supprimer toutes les entités en file d'attente
	 */
	void ClearWaitingLine();

	/**
	 * Obtenir l'élément XML d'un niveau
	 * @return element, premier niveau si niveau inexistant
	 */
	tinyxml2::XMLElement* GetLevelElement(size_t level) const;

	/**
	 * "#FF0000" -> sf::Color(255, 0, 0)
	 */
	static sf::Color HexaToColor(const std::string& hexcolor);

	struct EntitySlot
	{
		Entity* entity;
		float spawntime;
	};

	float last_insert_time_;
	tinyxml2::XMLDocument doc_;
	// Level definition nodes
	std::vector<tinyxml2::XMLElement*> levels_;
	// Functions declarations nodes
	std::map<std::string, tinyxml2::XMLElement*> functions_;
	// liste des vaisseaux à spawner pour le niveau courant
	std::queue<EntitySlot> waiting_line_;
	size_t current_level_;
	size_t last_unlocked_level_;
	int total_points_;
};

#endif // LEVELMANAGER_HPP

