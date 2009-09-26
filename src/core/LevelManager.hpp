#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include <queue>
#include <string>

#include "../tinyxml/tinyxml.h"
#include "../entities/Entity.hpp"

/**
 * Singleton du gestionnaire de niveaux (file d'attente des unités à venir)
 */
class LevelManager
{
public:
	enum Error
	{
		SUCCESS ,
		FILE,   // Erreur d'e-s
		UNDEF,  // Niveau non défini
		PARSE  // Erreur de parsing
	};

	/**
	 * Récupérer l'instance unique
	 * @return référence sur le gestionnaire de niveaux
	 */
	static LevelManager& GetInstance();

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
	 * @param level: numéro du niveau
	 */
	Error SetCurrent(int level);

	/**
	 * Obtenir la description d'un niveau
	 * @param level: numéro du niveau
	 * @return chaîne de description
	 */
	const char* GetDescription(int level) const;

	/**
	 * Obtenir la couleur de fond haut
	 */
	sf::Color GetTopColor(int level) const;

	/**
	 * Obtenir la couleur de fond bas
	 */
	sf::Color GetBottomColor(int level) const;

	/**
	 * Obtenir le nombre de niveaux chargés
	 */
	int CountLevel() const;

private:
	LevelManager();
	LevelManager(const LevelManager&);
	~LevelManager();

	/**
	 * Parser un fichier de niveaux
	 */
	Error ParseFile(const char* file);

	Error ParseLevel(TiXmlElement* elem);

	void ParseEntity(TiXmlElement* elem);

	void AppendToWaitingLine(Entity* entity, float time);

	void Purge();

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

	// pointeurs vers les définitions XML des niveaux
	std::vector<TiXmlElement*> levels_;
	TiXmlDocument doc_;
	// liste des vaisseaux à spawner pour le niveau courant
	std::queue<EntitySlot> waiting_line_;
	float last_insert_time_;
};

#endif // LEVELMANAGER_HPP

