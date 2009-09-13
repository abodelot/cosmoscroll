#include <iostream>
#include <cassert>

#include "LevelManager.hpp"

#include "../entities/EntityManager.hpp"
#include "../entities/Asteroid.hpp"
#include "../entities/EvilBoss.hpp"
#include "Game.hpp"
#include "../utils/Misc.hpp"

#define LEVEL_FILE "data/levels/levels.xml"


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
	EntityManager& entity_manager = EntityManager::GetInstance();

	elem = elem->FirstChildElement();
	Entity* player = Game::GetInstance().GetPlayerShip();

	if (elem == NULL)
	{
		std::cerr << "Impossible d'atteindre le noeud. Dying." << std::endl;
		exit(EXIT_FAILURE);
	}

	EntitySlot slot;
	sf::Vector2f offset;
	float t = 0.0f, last_t = 0.0f;
	std::string classname;

	printf("parsing level...\n");

	while (elem)
	{
		classname = elem->Value();

		// attributs commun à toutes les entités
		elem->QueryFloatAttribute("x", &offset.x);
		elem->QueryFloatAttribute("y", &offset.y);
		elem->QueryFloatAttribute("t", &t);
		last_t += t;

		slot.spawntime = last_t;

		if (classname == "ship")
		{
			int id = 0;
			elem->QueryIntAttribute("id", &id);
			slot.self = entity_manager.CreateSpaceShip(id, offset.x, offset.y);
		}
		else if (classname == "asteroid")
		{
			slot.self = new Asteroid(offset, Asteroid::BIG);
		}
		else if (classname == "evilboss")
		{
			slot.self = new EvilBoss(offset, player);
		}
		else
		{
			printf("Unimplemented element: %s\n", classname.c_str());
			abort();
		}

		waiting_line_.push(slot);
		elem = elem->NextSiblingElement();
	}
#ifdef DEBUG
	puts("done.");
#endif
	return SUCCESS;
}


Entity* LevelManager::GiveNext(float timer)
{
	if (!waiting_line_.empty())
	{
		if (waiting_line_.front().spawntime < timer)
		{
			Entity* next = waiting_line_.front().self;
			waiting_line_.pop();
			return next;
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

