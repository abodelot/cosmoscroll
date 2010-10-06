#include <iostream>

#include "LevelManager.hpp"
#include "../entities/EntityManager.hpp"
#include "../entities/Asteroid.hpp"
#include "../entities/EvilBoss.hpp"

#define DEFAULT_STARS_COUNT 33


LevelManager& LevelManager::GetInstance()
{
	static LevelManager self;
	return self;
}


LevelManager::LevelManager()
{
	last_insert_time_ = 0.f;
	current_level_ = last_unlocked_level_ = 1;
}


LevelManager::~LevelManager()
{
	ClearWaitingLine();
}


LevelManager::Error LevelManager::LoadCurrent()
{
	return ParseLevel(GetLevelElement(current_level_));
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
	if (level <= 0 || level > (int) levels_.size())
	{
		printf("warning: level %d is out of bounds, current level is now 1\n",
			level);
		level = 1;
		//std::cerr << "level " << level << " is undefined" << std::endl;
		//return UNDEF;
	}
	current_level_ = level;
	return SUCCESS;
}


int LevelManager::GetCurrent() const
{
	return current_level_;
}


void LevelManager::SetLastUnlocked(int level_num)
{
	if (level_num > 0 && level_num <= (int) levels_.size())
	{
		last_unlocked_level_ = level_num;
	}
	else
	{
		printf("warning: level %d is out of bounds, "
			"last unlocked level is now 1\n", level_num);
	}
}


int LevelManager::GetLastUnlocked() const
{
	return last_unlocked_level_;
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


int LevelManager::GetStarsCount() const
{
	int nb_stars = DEFAULT_STARS_COUNT;
	GetLevelElement(current_level_)->QueryIntAttribute("stars", &nb_stars);
	return nb_stars;
}


int LevelManager::GetDuration() const
{
	if (waiting_line_.empty())
	{
		return 0;
	}
	return waiting_line_.back().spawntime;
}


int LevelManager::CountLevel() const
{
	return levels_.size();
}


LevelManager::Error LevelManager::ParseFile(const char* file)
{
#ifdef DEBUG
	printf("* loading levels... ");
#endif
	if (!doc_.LoadFile(file))
	{
		std::cerr << "erreur lors du chargement de " << file << std::endl;
		std::cerr << "erreur #" << doc_.ErrorId() << " (" << doc_.ErrorDesc()
			<< ')' << std::endl;
		return FILE;
	}

	// Constitution de la map de pointeurs vers les fonctions
	TiXmlNode* node = doc_.RootElement()->FirstChild("functions")->FirstChild();
	while (node != NULL)
	{
		TiXmlElement* element = node->ToElement();
		const char* name = element->Attribute("name");
		if (name != NULL)
			functions_[name] = element;
		else
			puts("warning: function doesn't have a proper name");
		node = node->NextSibling();
	}

	// Constitution de la liste de pointeurs vers les niveaux
	node = doc_.RootElement()->FirstChild("levels")->FirstChild();
	while (node != NULL)
	{
		levels_.push_back(node->ToElement());
		node = node->NextSibling();
	}
#ifdef DEBUG
	printf("%d levels found\n", (int) levels_.size());
#endif
	return SUCCESS;
}


LevelManager::Error LevelManager::ParseLevel(TiXmlElement* elem)
{
	ClearWaitingLine();
	last_insert_time_ = 0.f;
	elem = elem->FirstChildElement();
	while (elem)
	{
		ParseEntity(elem);
		elem = elem->NextSiblingElement();
	}
	return SUCCESS;
}


void LevelManager::ParseEntity(TiXmlElement* elem)
{
	EntityManager& entity_mgr = EntityManager::GetInstance();
	const char* tag_name = elem->Value();
	if (strcmp(tag_name, "loop") == 0)
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
	else if (strcmp(tag_name, "call") == 0)
	{
		const char* func_name = elem->Attribute("func");
		if (func_name != NULL)
		{
			TiXmlElement* child = functions_[func_name]->FirstChildElement();
			while (child != NULL)
			{
				ParseEntity(child);
				child = child->NextSiblingElement();
			}
		}
		else
		{
			puts("missing func attribute");
		}
	}
	else if (strcmp(tag_name, "wait") == 0)
	{
		float t = 0.f;
		elem->QueryFloatAttribute("t", &t);
		last_insert_time_ += t;
	}
	else
	{
		sf::Vector2f position;
		position.x = 640; // default: screen right side
		float time = 0.f; // default: instant
		elem->QueryFloatAttribute("x", &position.x);
		elem->QueryFloatAttribute("y", &position.y);
		elem->QueryFloatAttribute("t", &time);
		Entity* entity = NULL;
		if (strcmp(tag_name, "ship") == 0)
		{
			int id = 0;
			elem->QueryIntAttribute("id", &id);
			entity = entity_mgr.CreateSpaceShip(id, position.x, position.y);
		}
		else if (strcmp(tag_name, "asteroid") == 0)
		{
			entity = new Asteroid(position, Asteroid::BIG);
		}
		else if (strcmp(tag_name, "evilboss") == 0)
		{
			entity = new EvilBoss(position);
		}
		else
		{
			printf("warning: undefined element '%s' while parsing level (ignored)\n", tag_name);
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


void LevelManager::ClearWaitingLine()
{
	// delete remaining entities in the waiting line
	while (!waiting_line_.empty())
	{
		delete waiting_line_.front().entity;
		waiting_line_.pop();
	}
}


TiXmlElement* LevelManager::GetLevelElement(int level) const
{
	--level; // first level is index 0
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

