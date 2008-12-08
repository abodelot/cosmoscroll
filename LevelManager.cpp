#include <iostream>
#include <cassert>

#include "LevelManager.hpp"

#include "Asteroid.hpp"
#include "Ennemy.hpp"
#include "EvilBoss.hpp"
#include "Game.hpp"
#include "Misc.hpp"

#define LEVEL_FILE "levels/levels.xml"


LevelManager& LevelManager::GetInstance()
{
	static LevelManager self;
	return self;
}


LevelManager::LevelManager()
{
   ParseFile(LEVEL_FILE);
}


LevelManager::Error LevelManager::ParseFile(const char* file)
{
	if (!doc_.LoadFile(file))
	{
		std::cerr << "erreur lors du chargement de " << file << std::endl;
		std::cerr << "erreur #" << doc_.ErrorId() << " (" << doc_.ErrorDesc()
			<< ')' << std::endl;
		return FILE;
	}
	
	// Constitution de la liste de pointeurs vers niveaux
#ifdef DEBUG 
	std::cout << "Building LevelManager List: " << std::flush;
#endif
	TiXmlNode* node = doc_.RootElement()->FirstChild();
	
	while (node != NULL)
	{
		levels_.push_back(node->ToElement());
		node = node->NextSibling();
	}
#ifdef DEBUG
	std::cout << levels_.size() << " elements." << std::endl;
#endif
	return SUCCESS;
}


LevelManager::Error LevelManager::Set(int level)
{
	Purge();
	--level;
	if (level < 0 || level >= (int) levels_.size())
	{
		std::cerr << "level " << level << " is undefined" << std::endl;
		return UNDEF;
	}
	return ParseLevel(levels_[level]);
}


const char* LevelManager::GetDescription(int level) const
{
	--level;
	assert(level >= 0 && level < (int) levels_.size());
	return levels_[level]->Attribute("desc");
}


LevelManager::Error LevelManager::ParseLevel(TiXmlElement* elem)
{
	elem = elem->FirstChild()->ToElement()->NextSiblingElement()->FirstChildElement();
	Entity* player = Game::GetInstance().GetShip();
	
	if (elem == NULL)
	{
		puts("Impossible d'atteindre le noeud. Dying.");
		exit(EXIT_FAILURE);
	}
	
	EntitySlot slot;
	sf::Vector2f offset;
	float t = 0.0f, last_t = 0.0f;
	std::string classname;
#ifdef DEBUG
	puts("------ begin level parsing ------");
#endif
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
		else if (classname == "EvilBoss")
		{
			slot.self = new EvilBoss(offset, player);
		}
		else
		{
			puts("Unimplemented element. Dying.");
			std::cerr << elem;
			exit(EXIT_FAILURE);
		}
		
		waiting_line_.push(slot);
#ifdef DEBUG
		printf("scheduled <%s> at %.2f sec\n", classname.c_str(), last_t);
#endif
		elem = elem->NextSiblingElement();
	}
#ifdef DEBUG
	puts("------ end level parsing ------");
#endif
	return SUCCESS;
}


Entity* LevelManager::GiveNext(float timer)
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


void LevelManager::Purge()
{
	while (!waiting_line_.empty())
	{
		Entity* entity = waiting_line_.front().self;
		delete entity;
		waiting_line_.pop();
	}
}


LevelManager::~LevelManager()
{
	Purge();
}

