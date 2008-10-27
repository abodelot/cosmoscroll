#include <iostream>

#include "Level.hpp"
#include "Misc.hpp"


#define LEVEL_FILE "levels.xml"
#define UNDEF_DESC "Level coudn't be loaded.\n This is a mostly serious error, heh.\n"

Level& Level::GetInstance()
{
    static Level self;
    return self;
}

Level::Level() 
{
    doc_ = new TiXmlDocument();
    LoadFile();
}

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

/*
 * Retourne l'entier codant pour le dernier level du fichier
 */
 
int Level::GetLastID()
{
    return last_id_;
}

/*
 * Retourne le groupe suivant
 */
 
Level::Error Level::GetNextGroup(Entity::ManagedContainer & cont_) 
{
  bool blocked = Blocks(); // Le prochain item aura Status::BLOCKS, et blocked repassera a false, s'il est à true là
  
  
  if (!current_node_)
  {
    std::cerr << "END: current_node_ vaut NULL" << std::endl;
    return END;
  }
  
  
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
  
  
}

Level::Error Level::LoadFile()
{
    Level::Error err = SUCCESS;
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


