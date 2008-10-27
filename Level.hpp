#ifndef LEVEL_HPP_
#define LEVEL_HPP_
#include <string>
#include "tinyxml/tinyxml.h"
#include "Entity.hpp"


class Level {

public:

enum Error {
            SUCCESS = 0,
            FILE,   // Erreur d'e-s
            UNDEF,  // Niveau non défini
            PARSE,  // Erreur de parsing
            END     // Fin de chaînage  
            };


/*
 * Récupérer l'instance unique
 * @return référence sur le gestionnaire de ressources
 */
static Level& GetInstance();

/*
 * Définit le level courant, affecte à Description le contenu de l'attribut description, 
 * s'îl est non vide.
 */
 
Level::Error SetLevel(int Level, std::string& Description);

/*
 * Retourne l'entier codant pour le dernier level du fichier
 */
 
int GetLastID();

/*
 * Retourne le groupe suivant sous forme (status, t, entité)
 * ou une valeur négative pour le t du/des premier(s) element(s), si on doit "bloquer" 
 * |t| secondes. (Exemple: on spawn 5 sec après la mort du dernier ennemi à l'ecran ->
 *        < -5, Entity> )
 */
 
Level::Error GetNextGroup(Entity::ManagedContainer &);


private:
Level();

Level::Error LoadFile();

int last_loaded_;
int last_id_;

TiXmlDocument* doc_;
TiXmlNode* level_;        // Pointera sur le Niveau en cours.
TiXmlNode* current_node_; // Pointera sur le groupe courant.

bool Blocks();

};


#endif // LEVEL_HPP_
