#include <iostream>

#include "Level.hpp"
#include "Misc.hpp"
#include "Asteroid.hpp"
#include "Ennemy.hpp"
#include "Game.hpp"


#define UNDEF_DESC "Level coudn't be loaded.\n This is a mostly serious error, heh.\n"


Level& Level::GetInstance()
{
    static Level self;
    return self;
}


Entity* Level::GiveNext(float timer)
{
    if (!waiting_line_.empty())
    {
        EntitySlot next = waiting_line_.front();
        if (next.spawntime < timer)
        {
            waiting_line_.pop();
            return next.self;
        }
    }
    return NULL;
}


int Level::RemainingEntities() const
{
    return waiting_line_.size();
}



Level::Level() 
{
    //Load(LEVEL_FILE);
}


bool Level::Load(const char* filename)
{
    // purge 
    while (!waiting_line_.empty())
    {
        waiting_line_.pop();
    }
    
    if (doc_.LoadFile(filename))
    {
        return Parse();
    }
    std::cerr << "erreur lors du chargement de " << filename << std::endl;
    std::cerr << "erreur #" << doc_.ErrorId() << " (" << doc_.ErrorDesc() << ')'
        << std::endl;
    exit(EXIT_FAILURE);
    return false;
}


bool Level::Parse()
{
    Entity* player = Game::GetInstance().GetPlayer();
    TiXmlHandle handle(&doc_);
    TiXmlElement *elem = handle.FirstChildElement().FirstChildElement().Element();
    
    if(elem == NULL)
    {
        std::cerr << "Impossible d'atteindre le noeud. DIE." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    EntitySlot slot;
    sf::Vector2f offset;
    float t = 0.0f, last_t = 0.0f;
    std::string classname;
    puts("------ begin level ------");
    while (elem)
    {
	    classname = elem->Attribute("name");
        elem->QueryFloatAttribute("x", &offset.x);
        elem->QueryFloatAttribute("y", &offset.y);
        elem->QueryFloatAttribute("t", &t);
        last_t += t;
    
        slot.spawntime = last_t;
        
        if (classname == "Blorb")
        {
            slot.self = Ennemy::Make(Ennemy::BLORB, offset, player);
        }
        else if (classname == "Interceptor")
        {
            slot.self = Ennemy::Make(Ennemy::INTERCEPTOR, offset, player);
        }
        else if (classname == "Drone")
        {
            slot.self = Ennemy::Make(Ennemy::DRONE, offset, player);
        }
        else if (classname == "Asteroid")
        {
            slot.self = new Asteroid(offset, Asteroid::BIG);
	    }
        else
        {
            puts("Invalid bad guy name. DIE");
            exit(EXIT_FAILURE);
        }
        
        waiting_line_.push(slot);
        printf("scheduled <%s> at %.2f sec\n", classname.c_str(), last_t);
        
        elem = elem->NextSiblingElement();
    }
    puts("------ end level ------");
    return true;
}


/*
Level::Error Level::SetLevel(int level, std::string& Description)
{
    Level::Error err;
    TiXmlNode* node = doc_->RootElement()->FirstChild(); // Level 1

    while (node && level > 1)
    {
        node = node->NextSibling();
        -- level;
    }
    if (node)
    {
        err = SUCCESS;
        Description = node->ToElement()->Attribute("desc");
        find_replace(Description, "\\n", "\n");
        level_ = node;
        current_node_ = node->FirstChild("decl")->FirstChild();
        if (!current_node_)
        {
        err = PARSE;
        }
    }
    else
    {
        level_ = current_node_ = NULL;
        err = UNDEF;
        Description = UNDEF_DESC;
    }
    return err;
}
*/
/*
 * Retourne l'entier codant pour le dernier level du fichier
 */
 /*
int Level::GetLastID()
{
	if (last_id_ == 0)
	{
		TiXmlNode* node = doc_->RootElement()->FirstChild(); // Level 1
		int count = 1;
	    while (node)
	    {
	        node = node->NextSibling();
	        ++ count;
	    }
		last_id_ = count - 1;
	}
	return last_id_;
}
*/
/*
 * Retourne le groupe suivant
 */
 /*
Level::Error Level::GetNextGroup(Entity::ManagedContainer & cont_) 
{
 bool blocked = Blocks(); // Le prochain item aura Status::BLOCKS, et blocked repassera a false, s'il est à true là
  Level::Error err = SUCCESS;
  if (!current_node_)
  {
    std::cerr << "END: current_node_ vaut NULL" << std::endl;
    err = END;
  }
  else
  {
  
	TiXmlElement* current_element_ = current_node_->ToElement();
*/  
  /*
    Si balise GROUP ouvrante
        -> on lit x y t
        
    Sinon si balise ITEM ouvrante
        -> on renvoie et consomme Item (Sortie de la methode)
  */
  
  
  /*
    -> On a eu une balise GROUP ouvrante
    
     * WHILE (balise GROUP pas fermée)
         -> On tombe sur un CALL:
                ExpandCall()
         -> On tombe sur un ITEM:
                ExpandItem()
  */
/*  }
  return err;
}
*/
/*
Level::Error Level::LoadFile()
{
    Level::Error err = SUCCESS;
    last_id_ = 0;
    bool load_ok = doc_->LoadFile(LEVEL_FILE);
    if (!load_ok)
    {
        std::cerr << "Error loading " << LEVEL_FILE << std::endl;
        std::cerr << doc_->ErrorDesc() << std::endl << "@ Row:"  << doc_->ErrorRow() << ", Col:"
            << doc_->ErrorCol() << std::endl;
        err = PARSE;
    }
    else 
    {
        TiXmlElement* elem = doc_->RootElement();
        TiXmlAttribute* attrib = elem->FirstAttribute();
            
    #ifdef DEBUG
        std::cerr << "Fichier XML Charge: " << std::endl;
        std::cerr << "Nombre de niveaux: " << attrib->IntValue() << std::endl;
        std::cerr << "Pack: " << (attrib->Next())->Value() << std::endl;
    #endif
    }
    return err;
}

bool Level::Blocks()
{
// Retourne TRUE si balise <Block /> rencontrée

    return false;
}
*/

