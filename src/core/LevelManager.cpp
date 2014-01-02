#include <iostream>

#include "LevelManager.hpp"
#include "Constants.hpp"
#include "Resources.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Asteroid.hpp"
#include "entities/Spaceship.hpp"
#include "entities/bosses/SplitBoss.hpp"
#include "entities/bosses/EvilBoss.hpp"
#include "entities/bosses/TentaculatBoss.hpp"
#include "entities/bosses/BrainBoss.hpp"
#include "entities/decors/Gate.hpp"
#include "entities/decors/Canon.hpp"
#include "entities/decors/GunTower.hpp"
#include "utils/Error.hpp"
#include "utils/sfml_helper.hpp"


LevelManager& LevelManager::getInstance()
{
	static LevelManager self;
	return self;
}


LevelManager::LevelManager():
	m_current_level(1),
	m_last_unlocked_level(1),
	m_last_insert_time(0.f),
	m_total_points(0)
{
}


LevelManager::~LevelManager()
{
	resetSpawnQueue();
}


void LevelManager::loadLevelFile(const std::string& path)
{
	// Open level file
	if (m_xml_doc.LoadFile(path.c_str()) != 0)
	{
		Error::log << "Cannot load levels:\n" << path << "\n" << m_xml_doc.GetErrorStr1();
		throw Error::exception();
	}
	tinyxml2::XMLElement* root = m_xml_doc.RootElement();

	// Parse function nodes
	tinyxml2::XMLElement* node = root->FirstChildElement("functions")->FirstChildElement();
	while (node != NULL)
	{
		const char* name = node->Attribute("name");
		if (name != NULL)
			m_functions[name] = node;
		else
			std::cerr << "[levels] a function without a name has been ignored" << std::endl;
		node = node->NextSiblingElement();
	}

	// Parse level nodes
	node = root->FirstChildElement("levels")->FirstChildElement("level");
	while (node != NULL)
	{
		m_levels.push_back(node);
		node = node->NextSiblingElement("level");
	}
}


void LevelManager::initCurrentLevel()
{
	resetSpawnQueue();

	const tinyxml2::XMLElement* elem = getCurrentLevelElement()->FirstChildElement();
	while (elem)
	{
		parseEntities(elem);
		elem = elem->NextSiblingElement();
	}
}


Entity* LevelManager::spawnNextEntity(float elapsed_time)
{
	if (!m_spawn_queue.empty() && m_spawn_queue.front().spawntime < elapsed_time)
	{
		Entity* entity = m_spawn_queue.front().entity;
		m_spawn_queue.pop();
		return entity;
	}
	return NULL;
}


size_t LevelManager::getSpawnQueueSize() const
{
	return m_spawn_queue.size();
}


void LevelManager::setCurrent(size_t level)
{
	if (level < 1 || level > m_last_unlocked_level || level > m_levels.size())
	{
		std::cerr << " [levels] level " << level << " is undefined or still locked, using level 1 instead" << std::endl;
		level = 1;
	}
	m_current_level = level;
}


size_t LevelManager::getCurrent() const
{
	return m_current_level;
}


size_t LevelManager::unlockNextLevel()
{
	if (m_last_unlocked_level < m_levels.size())
	{
		m_current_level = ++m_last_unlocked_level;
	}
	return m_current_level;
}


size_t LevelManager::getLastUnlocked() const
{
	return m_last_unlocked_level;
}


void LevelManager::setLastUnlocked(size_t level)
{
	if (level < 1 || level > m_levels.size())
	{
		std::cerr << " [levels] level " << level << " is undefined, using level 1 instead" << std::endl;
		level = 1;
	}
	m_last_unlocked_level = level;
}


size_t LevelManager::getLevelCount() const
{
	return m_levels.size();
}


const char* LevelManager::getQuote() const
{
	return getCurrentLevelElement()->Attribute("quote");
}


const sf::Texture* LevelManager::getLayerImage1() const
{

	const char* p = getCurrentLevelElement()->Attribute("layer1");
	return p != NULL ? &Resources::getTexture(p) : NULL;
}


const sf::Texture* LevelManager::getLayerImage2() const
{
	const char* p = getCurrentLevelElement()->Attribute("layer2");
	return p != NULL ? &Resources::getTexture(p) : NULL;
}


sf::Color LevelManager::getLayerColor() const
{
	const char* p = getCurrentLevelElement()->Attribute("color");
	return p != NULL ? xsf::hexa_to_color(p) : sf::Color::White;
}


int LevelManager::getDecorHeight() const
{
	int height = 0;
	getCurrentLevelElement()->QueryIntAttribute("decor_height", &height);
	return height;
}


int LevelManager::getStarsCount() const
{
	int nb_stars = 0;
	getCurrentLevelElement()->QueryIntAttribute("stars", &nb_stars);
	return nb_stars;
}


const char* LevelManager::getMusicName() const
{
	return getCurrentLevelElement()->Attribute("music");
}


float LevelManager::getDuration() const
{
	return m_spawn_queue.empty() ? 0 : m_spawn_queue.back().spawntime;
}


int LevelManager::getTotalPoints() const
{
	return m_total_points;
}


void LevelManager::parseEntities(const tinyxml2::XMLElement* elem)
{
	const char* tag_name = elem->Value();
	// Loop tag: repeat the inner tags 'count' times
	if (strcmp(tag_name, "loop") == 0)
	{
		int count = 0;
		elem->QueryIntAttribute("count", &count);
		while (count > 0)
		{
			const tinyxml2::XMLElement* child = elem->FirstChildElement();
			while (child != NULL)
			{
				parseEntities(child);
				child = child->NextSiblingElement();
			}
			--count;
		}
	}
	// Call tag: insert the inner tags from a function node
	else if (strcmp(tag_name, "call") == 0)
	{
		const char* func_name = elem->Attribute("func");
		if (func_name != NULL)
		{
			NodeMap::const_iterator it = m_functions.find(func_name);
			if (it != m_functions.end())
			{
				const tinyxml2::XMLElement* child = it->second->FirstChildElement();
				while (child != NULL)
				{
					parseEntities(child);
					child = child->NextSiblingElement();
				}
			}
			else
			{
				std::cerr << "[levels] undefined function '" << func_name << "' in 'call' tag" << std::endl;
			}
		}
		else
		{
			std::cerr << "[levels] 'func' attribute missing in 'call' tag" << std::endl;
		}
	}
	// Wait tag: delay the next entity
	else if (strcmp(tag_name, "wait") == 0)
	{
		float t = 0.f;
		elem->QueryFloatAttribute("t", &t);
		m_last_insert_time += t;
	}
	// Entity tags
	else
	{
		// Parse attributes shared by all tags
		sf::Vector2f position(APP_WIDTH - 1.f, 0.f); // default x: screen right side
		float time = 0.f; // default time: no delay
		elem->QueryFloatAttribute("x", &position.x);
		elem->QueryFloatAttribute("y", &position.y);
		elem->QueryFloatAttribute("t", &time);
		Entity* entity = NULL;
		if (strcmp(tag_name, "ship") == 0)
		{
			// Get spaceship profile id
			const char* id = elem->Attribute("id");
			if (id != NULL)
			{
				Spaceship* spaceship = EntityManager::getInstance().createSpaceship(id);
				if (spaceship != NULL)
					m_total_points += spaceship->getPoints();

				entity = spaceship;
			}
		}
		else if (strcmp(tag_name, "asteroid") == 0)
		{
			entity = new Asteroid(Asteroid::BIG);
		}
		else if (strcmp(tag_name, "boss") == 0)
		{
			if      (elem->Attribute("id", "tentaculat")) entity = new TentaculatBoss();
			else if (elem->Attribute("id", "split"))      entity = new SplitBoss();
			else if (elem->Attribute("id", "brain"))      entity = new BrainBoss();
			else if (elem->Attribute("id", "evil"))       entity = new EvilBoss();
			else
				std::cerr << "[levels] unknown boss id '" << elem->Attribute("id") << "' ignored" << std::endl;
		}
		else if (strcmp(tag_name, "decor") == 0)
		{
			if      (elem->Attribute("id", "gate"))      entity = new Gate();
			else if (elem->Attribute("id", "canon"))     entity = new Canon();
			else if (elem->Attribute("id", "guntower"))  entity = new GunTower();
			else
				std::cerr << "[levels] unknown decor id '" << elem->Attribute("id") << "' ignored" << std::endl;
		}
		else
		{
			std::cerr << "[levels] unknown tag '" << tag_name << "' ignored" << std::endl;
		}

		if (entity != NULL)
		{
			entity->setPosition(position);
			appendToSpawnQueue(entity, time);
		}
		else
		{
			std::cerr << "[levels] invalid entity '" << tag_name << "' ignored" << std::endl;
		}
	}
}


void LevelManager::appendToSpawnQueue(Entity* entity, float delay)
{
	EntitySlot slot;
	slot.entity = entity;
	m_last_insert_time += delay;
	slot.spawntime = m_last_insert_time;
	m_spawn_queue.push(slot);
}


void LevelManager::resetSpawnQueue()
{
	// Delete remaining entities in the spawn queue
	while (!m_spawn_queue.empty())
	{
		delete m_spawn_queue.front().entity;
		m_spawn_queue.pop();
	}

	m_last_insert_time = 0.f;
	m_total_points = 0;
}


const tinyxml2::XMLElement* LevelManager::getCurrentLevelElement() const
{
	return m_levels.at(m_current_level - 1);
}
