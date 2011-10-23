#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <queue>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>

#include "tinyxml/tinyxml.h"

class Entity;

/**
 * Singleton du gestionnaire de niveaux (file d'attente des unités à venir)
 */
class LevelManager
{
public:
	/**
	 * Récupérer l'instance unique
	 * @return référence sur le gestionnaire de niveaux
	 */
	static LevelManager& GetInstance();

	/**
	 * Parser un fichier de niveaux
	 */
	void ParseFile(const std::string& file, unsigned int offset = 0);

	void LoadCurrent();

	/**
	 * Obtenir la prochaine unité du niveau
	 * @param timer: temps écoulé depuis le début du niveau
	 * @return entity s'il y en a au moins une qui doit aparaître avant timer
	 * sinon, NULL (il n'y pas d'entity avant timer)
	 */
	Entity* GiveNextEntity(float timer);

	/**
	 * Obtenir le nombre d'unités restantes dans la file d'attente du niveau
	 */
	inline int RemainingEntities() const
	{
		return waiting_line_.size();
	};

	/**
	 * Définir le niveau courant
	 * @param level_num: numéro du niveau
	 */
	void SetCurrent(int level_num);
	void VerifyCurrent(void);
	int GetCurrent() const;


	/**
	 * Last reached level
	 */
	int UnlockNextLevel();
	void SetLastUnlocked(int level);
	int GetLastUnlocked() const;

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
	const sf::Image* GetBackgroundImage() const;

	int GetStarsCount() const;

	/**
	 * Durée du niveau en secondes
	 */
	int GetDuration() const;


	/**
	 * Obtenir le nombre de niveaux chargés
	 */
	int CountLevel() const;

	/**
	 * @return true si tous les niveaux sont terminés
	 */
	bool AllLevelsCompleted() const;

	/**
	 * Activer/désactiver le mode hardcore
	 */
	void EnableHardcore(bool hardcore);

	/**
	 * @return true si le mode hardcore est activé
	 */
	bool IsHardcoreEnabled() const;

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
	void ParseLevel(TiXmlElement* elem);

	/**
	 * Parser un élément de niveau
	 */
	void ParseEntity(TiXmlElement* elem);

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
	TiXmlElement* GetLevelElement(int level) const;

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
	TiXmlDocument doc_;
	// pointeurs vers les définitions XML des niveaux
	std::vector<TiXmlElement*> levels_;
	// fonctions XML des niveaux
	std::map<std::string, TiXmlElement*> functions_;
	// liste des vaisseaux à spawner pour le niveau courant
	std::queue<EntitySlot> waiting_line_;
	int current_level_;
	int last_unlocked_level_;
	int total_points_;
};

#endif // LEVELMANAGER_HPP

