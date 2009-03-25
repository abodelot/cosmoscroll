#include <iostream>

#include "EntityManager.hpp"
#include "../tinyxml/tinyxml.h"
#include "../utils/MediaManager.hpp"

#define WEAPON_DEFINITIONS "data/xml/weapons.xml"


EntityManager& EntityManager::GetInstance()
{
	static EntityManager self;
	return self;
}


EntityManager::EntityManager()
{
	last_id_ = 0;
	LoadWeapons(WEAPON_DEFINITIONS);
}


EntityManager::~EntityManager()
{
	Clear();
}


void EntityManager::Update(float frametime)
{
#if 0
	EntityList::iterator it, it2;

	// removing dead entities
	for (it = entities_.begin(); it != entities_.end();)
	{
		if ((**it).IsDead())
		{
			delete *it;
			it = entities_.erase(it);
		}
		else
		{
			++it;
		}
	}

	// update and collision
	sf::FloatRect rect, rect2;
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		(**it).Update(frametime);
		(**it).GetCollideRect(rect);
		it2 = it;
		for (++it2; it2 != entities_.end(); ++it2)
		{
			(**it2).GetCollideRect(rect);
			if (rect.Intersects(rect2))
			{
				(**it).OnCollide(**it2);
				(**it2).OnCollide(**it);
			}
		}
	}
#endif
}


void EntityManager::AddEntity(Entity* entity)
{
	entity->SetID(last_id_++);
	entities_.push_front(entity);
}


void EntityManager::Clear()
{
	// suppression de toutes les entités
	for (EntityList::iterator it = entities_.begin(); it != entities_.end();
		++it)
	{
		delete *it;
	}
	entities_.clear();
}


Weapon EntityManager::BuildWeapon(int id) const
{
	WeaponMap::const_iterator it;
	it = weapon_defs_.find(id);

	if (it != weapon_defs_.end())
	{
		const WeaponDefinition& def = it->second;
		Weapon weapon(*def.image, def.fire_rate, def.heat_cost, def.damage, def.speed);
		return weapon;
	}
	abort();
}



void EntityManager::Render(sf::RenderTarget& target) const
{
	// affichage de toutes les entités
	for (EntityList::const_iterator it = entities_.begin();
		it != entities_.end(); ++it)
	{
		target.Draw(**it);
	}
}


void EntityManager::LoadWeapons(const char* filename)
{
	const MediaManager& media = MediaManager::GetInstance();

	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		std::cerr << "can't load weapon definitions: " << filename << "\n";
		std::cerr << doc.ErrorDesc() << std::endl;
		abort();
	}
	TiXmlHandle handle(&doc);
	TiXmlElement* elem = handle.FirstChildElement().FirstChildElement().Element();

	while (elem != NULL)
	{
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
			std::cerr << "weapon id is missing" << std::endl;
			abort();
		}
		WeaponDefinition* weapon = &weapon_defs_[id];

		const char* p = elem->Attribute("name");
		if (p == NULL)
		{
			std::cerr << "weapon name is missing" << std::endl;
			abort();
		}
		weapon->name = p;

		p = elem->Attribute("image");
		if (p == NULL)
		{
			std::cerr << "weapon image is missing" << std::endl;
			abort();
		}
		weapon->image = &media.GetImage(p);

		if (elem->QueryFloatAttribute("heat_cost", &weapon->heat_cost) != TIXML_SUCCESS)
		{
			std::cerr << "weapon heat cost is missing" << std::endl;
			abort();
		}

		if (elem->QueryFloatAttribute("fire_rate", &weapon->fire_rate) != TIXML_SUCCESS)
		{
			std::cerr << "weapon fire rate is missing" << std::endl;
			abort();
		}

		if (elem->QueryIntAttribute("damage", &weapon->damage) != TIXML_SUCCESS)
		{
			std::cerr << "weapon damage is missing" << std::endl;
			abort();
		}

		if (elem->QueryIntAttribute("speed", &weapon->speed) != TIXML_SUCCESS)
		{
			std::cerr << "weapon speed is missing" << std::endl;
			abort();
		}

		elem = elem->NextSiblingElement();
	}
}
