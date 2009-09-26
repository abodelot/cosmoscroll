#include <iostream>

#include "LevelManager.hpp"
#include "../entities/EntityManager.hpp"
#include "../entities/Asteroid.hpp"
#include "../entities/EvilBoss.hpp"

#define LEVEL_FILE "data/levels/levels.xml"


LevelManager& LevelManager::GetInstance()
{
	static LevelManager self;
	return self;
}


LevelManager::LevelManager()
{
	last_insert_time_ = 0.f;
	ParseFile(LEVEL_FILE);
}


LevelManager::~LevelManager()
{
	Purge();
}


Entity* LevelManager::GiveNextEntity(float timer)
{
	if (!waiting_line_.empty())
	{
		if (waiting_line_.front().spawntime < timer)
		{
			Entity* next = waiting_line_.front().entity;
			waiting_line_.pop();
			return next;
		}
	}
	return NULL;
}


LevelManager::Error LevelManager::SetCurrent(int level)
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
	return GetLevelElement(level)->Attribute("desc");
}


sf::Color LevelManager::GetTopColor(int level) const
{
	const char* p = GetLevelElement(level)->Attribute("bg_top");
	return p != NULL ? HexaToColor(p) : sf::Color::Black;
}


sf::Color LevelManager::GetBottomColor(int level) const
{
	const char* p = GetLevelElement(level)->Attribute("bg_bottom");
	return p != NULL ? HexaToColor(p) : sf::Color::Black;
}


int LevelManager::CountLevel() const
{
	return levels_.size();
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
	std::cout << "Building LevelManager list ... " << std::flush;
#endif
	TiXmlNode* node = doc_.RootElement()->FirstChild();

	while (node != NULL)
	{
		levels_.push_back(node->ToElement());
		node = node->NextSibling();
	}
#ifdef DEBUG
	std::cout << levels_.size() << " levels found" << std::endl;
#endif
	return SUCCESS;
}


/*LevelManager::Error LevelManager::ParseLevel(TiXmlElement* elem)
{
	last_insert_time_ = 0.f;

	elem = elem->FirstChildElement();
	Entity* player = Game::GetInstance().GetPlayerShip();

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
}*/


LevelManager::Error LevelManager::ParseLevel(TiXmlElement* elem)
{
	last_insert_time_ = 0.f;
	elem = elem->FirstChildElement();
	printf("parsing level...\n");
	while (elem)
	{
		ParseEntity(elem);
		elem = elem->NextSiblingElement();
	}
	puts("done!");
	return SUCCESS;
}


void LevelManager::ParseEntity(TiXmlElement* elem)
{
	EntityManager& entity_mgr = EntityManager::GetInstance();
	const char* classname = elem->Value();
	if (strcmp(classname, "for") == 0)
	{
		int count = 0;
		elem->QueryIntAttribute("count", &count);
		while (count > 0)
		{
			TiXmlElement* child = elem->FirstChildElement();
			while (child != NULL)
			{
				ParseEntity(child);
				child = child->NextSiblingElement();
			}
			--count;
		}
	}
	else
	{
		sf::Vector2f position;
		float time = 0.f;
		elem->QueryFloatAttribute("x", &position.x);
		elem->QueryFloatAttribute("y", &position.y);
		elem->QueryFloatAttribute("t", &time);
		Entity* entity = NULL;
		if (strcmp(classname, "ship") == 0)
		{
			int id = 0;
			elem->QueryIntAttribute("id", &id);
			entity = entity_mgr.CreateSpaceShip(id, position.x, position.y);
		}
		else if (strcmp(classname, "asteroid") == 0)
		{
			entity = new Asteroid(position, Asteroid::BIG);
		}
		else if (strcmp(classname, "evilboss") == 0)
		{
			entity = new EvilBoss(position);
		}
		else
		{
			printf("warning: undefined element '%s' while parsing level (ignored)\n", classname);
		}
		AppendToWaitingLine(entity, time);
	}
}


void LevelManager::AppendToWaitingLine(Entity* entity, float time)
{
    EntitySlot slot;
    slot.entity = entity;
    last_insert_time_ += time;
    slot.spawntime = last_insert_time_;
    waiting_line_.push(slot);
}


void LevelManager::Purge()
{
	// désallocation des entités restées en file d'attente
	while (!waiting_line_.empty())
	{
		delete waiting_line_.front().entity;
		waiting_line_.pop();
	}
}


TiXmlElement* LevelManager::GetLevelElement(int level) const
{
    --level;
    if (level < 0 || level >= (int) levels_.size())
    {
        printf("info: level %d is not a valid level, using first level instead\n", level + 1);
        level = 0;
    }
    return levels_[level];
}


sf::Color LevelManager::HexaToColor(const std::string& hexcolor)
{
	sf::Color color = sf::Color::Black;
	if (hexcolor.size() == 7)
	{
		color.r = strtoul(hexcolor.substr(1, 2).c_str(), NULL, 16);
		color.g = strtoul(hexcolor.substr(3, 2).c_str(), NULL, 16);
		color.b = strtoul(hexcolor.substr(5, 2).c_str(), NULL, 16);
	}
	return color;
}

