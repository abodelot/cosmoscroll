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
	LoadSpaceShips("data/xml/spaceships.xml");
}


EntityManager::~EntityManager()
{
	Clear();

	SpaceShipMap::iterator it;
	for (it = spaceships_defs_.begin(); it != spaceships_defs_.end(); ++it)
	{
		delete it->second;
	}
}


void EntityManager::Update(float frametime)
{
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
			(**it2).GetCollideRect(rect2);
			if (rect.Intersects(rect2))
			{
				(**it).OnCollide(**it2);
				(**it2).OnCollide(**it);
			}
		}
	}
}


void EntityManager::AddEntity(Entity* entity)
{
	entity->SetID(++last_id_);
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


int EntityManager::Count() const
{
	return entities_.size();
}


Weapon EntityManager::BuildWeapon(int id) const
{
	WeaponMap::const_iterator it;
	it = weapon_defs_.find(id);

	if (it != weapon_defs_.end())
	{
		const WeaponDefinition& def = it->second;
		Weapon weapon(*def.image, def.fire_rate, def.heat_cost, def.damage, def.speed, def.sound.c_str());
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

		// sound (optional)
		p = elem->Attribute("sound");
		weapon->sound = p == NULL ? "" : p;


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


void EntityManager::LoadSpaceShips(const char* filename)
{

	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		std::cerr << "can't load space ships definitions: " << filename << "\n";
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
			std::cerr << "space ship id is missing" << std::endl;
			abort();
		}

		const char* name = elem->Attribute("name");
		if (name == NULL)
		{
			std::cerr << "space ship name is missing" << std::endl;
			abort();
		}

		const char* animation = elem->Attribute("animation");
		if (animation == NULL)
		{
			std::cerr << "space ship animation is missing" << std::endl;
			abort();
		}

		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
			std::cerr << "space ship HP is missing" << std::endl;
			abort();
		}

		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
		{
			std::cerr << "space ship speed is missing" << std::endl;
			abort();
		}

		SpaceShip* ship = new SpaceShip(animation, hp, speed);

		TiXmlElement* weapon = elem->FirstChildElement();
		if (weapon != NULL)
		{
			int w_id, w_x, w_y;
			if (weapon->QueryIntAttribute("id", &w_id) != TIXML_SUCCESS)
			{
				std::cerr << "ship weapon id not found" << std::endl;
				abort();
			}

			if (weapon->QueryIntAttribute("x", &w_x) != TIXML_SUCCESS)
			{
				std::cerr << "ship weapon offset x not found" << std::endl;
				abort();
			}

			if (weapon->QueryIntAttribute("y", &w_y) != TIXML_SUCCESS)
			{
				std::cerr << "ship weapon offset y not found" << std::endl;
				abort();
			}
			Weapon* weapon_object = new Weapon(BuildWeapon(w_id));
			weapon_object->SetOffset(w_x, w_y);
			ship->SetWeapon(weapon_object);

		}

		spaceships_defs_[id] = ship;

		elem = elem->NextSiblingElement();
	}
}

SpaceShip* EntityManager::CreateSpaceShip(int id, int x, int y)
{
	SpaceShipMap::const_iterator it;
	it = spaceships_defs_.find(id);

	if (it != spaceships_defs_.end())
	{
		SpaceShip* ship = it->second->Clone();
		ship->SetPosition(x, y);
		return ship;
	}
	abort();
	return NULL;
}
