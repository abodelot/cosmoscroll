#include <iostream>

#include "LevelManager.hpp"
#include "Game.hpp"
#include "Resources.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Asteroid.hpp"
#include "entities/Spaceship.hpp"
#include "entities/bosses/SplitBoss.hpp"
#include "entities/bosses/EvilBoss.hpp"
#include "entities/bosses/BossTentacles.hpp"
#include "entities/complex/Gate.hpp"
#include "entities/complex/Canon.hpp"
#include "entities/complex/GunTower.hpp"
#include "utils/Error.hpp"

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
	total_points_ = 0;
}


LevelManager::~LevelManager()
{
	ClearWaitingLine();
}


void LevelManager::LoadCurrent()
{
	ParseLevel(GetLevelElement(current_level_));
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

void LevelManager::SetCurrent(int level)
{
	current_level_ = level;
}

void LevelManager::VerifyCurrent(void)
{
	if (current_level_ < 1 || current_level_ > last_unlocked_level_ || current_level_ > (int) levels_.size())
	{
		std::cerr << " [levels] level " << current_level_ << " is undefined, using level 1" << std::endl;
		current_level_ = 1;
	}
}


int LevelManager::GetCurrent() const
{
	return current_level_;
}


int LevelManager::unlockNextLevel()
{
	if (last_unlocked_level_ < (int) levels_.size())
	{
		current_level_ = ++last_unlocked_level_;
	}
	return current_level_;
}


int LevelManager::getLastUnlocked() const
{
	return last_unlocked_level_;
}


void LevelManager::setLastUnlocked(int level)
{
	if (level < 1 || level > (int) levels_.size())
	{
		std::cerr << " [levels] level " << level << " is undefined, using level 1" << std::endl;
		level = 1;
	}
	last_unlocked_level_ = level;
}


const char* LevelManager::GetDescription() const
{
	return GetLevelElement(current_level_)->Attribute("desc");
}


const sf::Texture* LevelManager::GetLayerImage1() const
{

	const char* p = GetLevelElement(current_level_)->Attribute("layer1");
	return p != NULL ? &Resources::getTexture(p) : NULL;
}


const sf::Texture* LevelManager::GetLayerImage2() const
{
	const char* p = GetLevelElement(current_level_)->Attribute("layer2");
	return p != NULL ? &Resources::getTexture(p) : NULL;
}


sf::Color LevelManager::GetLayerColor() const
{
	const char* p = GetLevelElement(current_level_)->Attribute("color");
	return p != NULL ? HexaToColor(p) : sf::Color::White;
}


int LevelManager::GetDecorHeight() const
{
	int height = 0;
	GetLevelElement(current_level_)->QueryIntAttribute("decor_height", &height);
	return height;
}


int LevelManager::GetStarsCount() const
{
	int nb_stars = DEFAULT_STARS_COUNT;
	GetLevelElement(current_level_)->QueryIntAttribute("stars", &nb_stars);
	return nb_stars;
}


const char* LevelManager::GetMusic() const
{
	return GetLevelElement(current_level_)->Attribute("music");
}


int LevelManager::GetDuration() const
{
	if (waiting_line_.empty())
	{
		return 0;
	}
	return waiting_line_.back().spawntime;
}


int LevelManager::GetTotalPoints() const
{
	return total_points_;
}


int LevelManager::CountLevel() const
{
	return levels_.size();
}


bool LevelManager::AllLevelsCompleted() const
{
	return last_unlocked_level_ > (int) levels_.size();
}


int LevelManager::ParseFile(const std::string& file)
{
	// Open level file
	if (doc_.LoadFile(file.c_str()) != 0)
	{
		Error::log << "Cannot load levels:\n" << file << "\n" << doc_.GetErrorStr1();
		throw Error::exception();
	}
	tinyxml2::XMLElement* root = doc_.RootElement();

	// Parse function nodes
	tinyxml2::XMLElement* node = root->FirstChildElement("functions")->FirstChildElement();
	while (node != NULL)
	{
		const char* name = node->Attribute("name");
		if (name != NULL)
			functions_[name] = node;
		else
			puts("warning: function doesn't have a proper name");
		node = node->NextSiblingElement();
	}

	// Parse level nodes
	node = root->FirstChildElement("levels")->FirstChildElement("level");
	while (node != NULL)
	{
		levels_.push_back(node);
		node = node->NextSiblingElement("level");
	}
	return (int) levels_.size();
}


void LevelManager::ParseLevel(tinyxml2::XMLElement* elem)
{
	ClearWaitingLine();
	last_insert_time_ = 0.f;
	total_points_ = 0;

	elem = elem->FirstChildElement();
	while (elem)
	{
		ParseEntity(elem);
		elem = elem->NextSiblingElement();
	}
}


void LevelManager::ParseEntity(tinyxml2::XMLElement* elem)
{
	EntityManager& entity_mgr = EntityManager::getInstance();
	const char* tag_name = elem->Value();
	if (strcmp(tag_name, "loop") == 0)
	{
		int count = 0;
		elem->QueryIntAttribute("count", &count);
		while (count > 0)
		{
			tinyxml2::XMLElement* child = elem->FirstChildElement();
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
			std::map<std::string, tinyxml2::XMLElement*>::iterator it = functions_.find(func_name);
			if (it != functions_.end())
			{
				tinyxml2::XMLElement* child	= it->second->FirstChildElement();
				while (child != NULL)
				{
					ParseEntity(child);
					child = child->NextSiblingElement();
				}
			}
			else
			{
				std::cerr << "[levels] invalid function '" << func_name << "' in 'call' tag" << std::endl;
			}
		}
		else
		{
			std::cerr << "[levels] 'func' attribute missing in 'call' tag" << std::endl;
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
		// common attributes for all tags
		sf::Vector2f position(0, 0);
		position.x = Game::WIDTH - 1; // default x: screen right side
		float time = 0.f; // default: no delay
		elem->QueryFloatAttribute("x", &position.x);
		elem->QueryFloatAttribute("y", &position.y);
		elem->QueryFloatAttribute("t", &time);
		Entity* entity = NULL;
		if (strcmp(tag_name, "ship") == 0)
		{
			int id = 0;
			elem->QueryIntAttribute("id", &id);
			Spaceship* spaceship = entity_mgr.CreateSpaceShip(id);
			total_points_ += spaceship->getPoints();
			entity = spaceship;
		}
		else if (strcmp(tag_name, "asteroid") == 0)
		{
			entity = new Asteroid(Asteroid::BIG);
		}
		else if (strcmp(tag_name, "evilboss") == 0)
		{
			entity = new EvilBoss();
		}
		else if (strcmp(tag_name, "boss_tentacles") == 0)
		{
			entity = new BossTentacles();
		}
		else if(strcmp(tag_name,"split_boss") == 0)
		{
		    entity = new SplitBoss();
		}
		else if (strcmp(tag_name, "decor") == 0)
		{
			if (elem->Attribute("id", "gate"))
				entity = new Gate();
			else if (elem->Attribute("id", "canon"))
				entity = new Canon();
			else if (elem->Attribute("id", "guntower"))
				entity = new GunTower();
		}
		else
		{
			std::cerr << "[levels] unsupported tag '" << tag_name << "' ignored" << std::endl;
		}
		entity->setPosition(position);
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
	// Delete remaining entities in the waiting line
	while (!waiting_line_.empty())
	{
		delete waiting_line_.front().entity;
		waiting_line_.pop();
	}
}


tinyxml2::XMLElement* LevelManager::GetLevelElement(int level) const
{
	--level; // index starts at 0
	if (level < 0 || level >= (int) levels_.size())
	{
		std::cerr << "[levels] index " << level << " is not a valid level, using index 0" << std::endl;
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
