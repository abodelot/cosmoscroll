#include <iostream>

#include "Level.hpp"
#include "Misc.hpp"
#include "Asteroid.hpp"
#include "Ennemy.hpp"
#include "Game.hpp"


Level::Level()
{
   ParseFile(LEVEL_FILE);
}

Level& Level::GetInstance()
{
    static Level self;
    return self;
}


Level::Error Level::ParseFile(const char* file)
{

    if (! doc_.LoadFile(file))
    {
        std::cerr << "erreur lors du chargement de " << file << std::endl;
        std::cerr << "erreur #" << doc_.ErrorId() << " (" << doc_.ErrorDesc() << ')'
            << std::endl;
    return FILE;
    }
    // Constitution de la liste de pointeurs vers niveaux
    #ifdef DEBUG 
    std::cerr << "Construction LevelList:" << std::flush;
    #endif
    TiXmlNode* node = doc_.RootElement()->FirstChild();
    
    while (node)
    {
        levels_.push_back(node->ToElement());
        node = node->NextSibling();
    }
    #ifdef DEBUG
    std:: cerr << levels_.size() << " niveaux." << std::endl;
    #endif
    return SUCCESS;
}

Level::Error Level::Set(int level, std::string& description)
{
   Purge();
   -- level;
   if ((level > 0) && level > static_cast<int>(levels_.size()))
   {
       return UNDEF;
   }
   
   description = levels_[level]->Attribute("desc");
   return ParseLevel(levels_[level]);
}

Level::Error Level::ParseLevel(TiXmlElement* elem)
{
    elem = elem->FirstChild()->ToElement()->NextSiblingElement()->FirstChildElement();    
    Entity* player = Game::GetInstance().GetPlayer();
    
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
            puts("Unimplemented element. Dying.");
            std::cerr << elem;
            exit(EXIT_FAILURE);
        }
        
        waiting_line_.push(slot);
        printf("scheduled <%s> at %.2f sec\n", classname.c_str(), last_t);
        
        elem = elem->NextSiblingElement();
    }
    puts("------ end level ------");
    return SUCCESS;
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



