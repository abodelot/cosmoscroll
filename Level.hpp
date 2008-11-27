#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <queue>
#include <string>

#include "tinyxml/tinyxml.h"
#include "Entity.hpp"

#define LEVEL_FILE			"levels.xml"

class Game;
	
class Level
{

public:

	enum Error
	{
				SUCCESS = 0,
				FILE,   // Erreur d'e-s
				UNDEF,  // Niveau non défini
				PARSE,  // Erreur de parsing
				END     // Fin de chaînage  
	};

    /*
     * Récupérer l'instance unique
     * @return référence sur le gestionnaire de niveaux
     */
    static Level& GetInstance();
    
    /*
     * Obtenir la prochaine unité du niveau
     * @param[in] timer: temps écoulé
     * @return entity s'il y en a au moins une qui doit aparaître avant timer
     * sinon, NULL (il n'y pas d'entity avant timer)
     */
    Entity* GiveNext(float timer);
    
    /*
     * Obtenir le nombre d'unités restantes dans la file d'attente du niveau
     */
    inline int RemainingEntities() const {
        return waiting_line_.size();
    };
    
    /*
     * Définit le level courant, affecte à Description le contenu de l'attribut description, 
     * s'îl est non vide.
     */
    Level::Error Set(int Level, std::string& Description);

    /*
     * Retourne l'entier codant pour le dernier level du fichier
     */
    inline int GetLastID() {
        return levels_.size();
    };
	
private:
    struct EntitySlot
    {
        Entity* self;
        float spawntime;
    };
    
    Level();
    
    Level::Error ParseFile(const char* file);
    
    Level::Error ParseLevel(TiXmlElement* elem);
    
    inline void Purge() {
      while (!waiting_line_.empty())
          waiting_line_.pop();
    };
    
    std::queue<EntitySlot> waiting_line_;
    std::vector<TiXmlElement*> levels_;
    TiXmlDocument doc_;
};

#endif // LEVEL_HPP
