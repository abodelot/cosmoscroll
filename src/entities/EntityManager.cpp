#include "EntityManager.hpp"
#include "Asteroid.hpp"
#include "../core/Game.hpp"
#include "../tinyxml/tinyxml.h"
#include "../utils/MediaManager.hpp"
#include "../utils/DIE.hpp"


EntityManager& EntityManager::GetInstance()
{
	static EntityManager self;
	return self;
}


EntityManager::EntityManager()
{
	uniques_.push_back(new Asteroid(sf::Vector2f(0, 0), Asteroid::BIG));
}


EntityManager::~EntityManager()
{
	Clear();

	for (size_t i = 0; i < uniques_.size(); ++i)
	{
		delete uniques_[i];
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
	entities_.push_back(entity);
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
		DIE("can't load weapon definitions: '%s' (%s)", filename, doc.ErrorDesc());
	}

	TiXmlElement* elem = doc.RootElement()->FirstChildElement();
	while (elem != NULL)
	{
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
			DIE("weapon id is missing");
		}
		WeaponDefinition* weapon = &weapon_defs_[id];

		const char* p = elem->Attribute("name");
		if (p == NULL)
		{
			DIE("weapon name is missing");
		}
		weapon->name = p;

		p = elem->Attribute("image");
		if (p == NULL)
		{
			DIE("weapon image is missing")
		}
		weapon->image = &media.GetImage(p);

		// sound (optional)
		p = elem->Attribute("sound");
		weapon->sound = p == NULL ? "" : p;

		if (elem->QueryFloatAttribute("heat_cost", &weapon->heat_cost) != TIXML_SUCCESS)
		{
			DIE("weapon heat cost is missing");
		}

		if (elem->QueryFloatAttribute("fire_rate", &weapon->fire_rate) != TIXML_SUCCESS)
		{
			DIE("weapon fire rate is missing");
		}

		if (elem->QueryIntAttribute("damage", &weapon->damage) != TIXML_SUCCESS)
		{
			DIE("weapon damage is missing");
		}

		if (elem->QueryIntAttribute("speed", &weapon->speed) != TIXML_SUCCESS)
		{
			DIE("weapon speed is missing");
		}

		elem = elem->NextSiblingElement();
	}
}


void EntityManager::LoadAnimations(const char* filename)
{
	// ppd
	// chargement des animations
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		DIE("can't open animation definitions: %s (%s)", filename, doc.ErrorDesc());
	}

	TiXmlElement* elem = doc.RootElement()->FirstChildElement();
	// attributs
	int width, height, count;
	float delay;
	const char* name;
	const char* img;
	while (elem != NULL)
	{
		bool ok = true;
		ok &= ((name = elem->Attribute("name")) != NULL);
		ok &= ((img = elem->Attribute("img")) != NULL);
		ok &= (elem->QueryIntAttribute("width", &width) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("height", &height) == TIXML_SUCCESS);
		ok &= (elem->QueryIntAttribute("count", &count) == TIXML_SUCCESS);
		ok &= (elem->QueryFloatAttribute("delay", &delay) == TIXML_SUCCESS);
		int x = 0, y = 0;
		elem->QueryIntAttribute("x", &x);
		elem->QueryIntAttribute("y", &y);

		if (ok)
		{
			// construction de l'animation
			Animation* p = &animations_[name];
			for (int i = 0; i < count; ++i)
			{
				p->AddFrame(x + i * width, y, width, height);
			}
			p->SetDelay(delay);
			p->SetImage(GET_IMG(img));
			printf("loading animation %s\n", name);
		}
		elem = elem->NextSiblingElement();
	}
	puts("animations loaded");
}


void EntityManager::LoadSpaceShips(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		DIE("can't load space ships definitions: '%s' (%s)", filename, doc.ErrorDesc());
	}

	TiXmlElement* elem = doc.RootElement()->FirstChildElement();
	while (elem != NULL)
	{
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
		{
			DIE("space ship id is missing");
		}

		const char* name = elem->Attribute("name");
		if (name == NULL)
		{
			DIE("space ship name is missing")
		}

		const char* animation = elem->Attribute("animation");
		if (animation == NULL)
		{
			DIE("space ship animation is missing");
		}

		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
		{
			DIE("space ship HP is missing")
		}

		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
		{
			DIE("space ship speed is missing");
		}

		SpaceShip* ship = new SpaceShip(animation, hp, speed);

		const char* move_pattern = elem->Attribute("move");
		const char* attack_pattern = elem->Attribute("attack");
		ship->SetMovePattern(move_pattern);
		ship->SetAttackPattern(attack_pattern);

		TiXmlElement* weapon = elem->FirstChildElement();
		if (weapon != NULL)
		{
			int w_id, w_x, w_y;
			if (weapon->QueryIntAttribute("id", &w_id) != TIXML_SUCCESS)
			{
				DIE("ship weapon id not found");
			}

			if (weapon->QueryIntAttribute("x", &w_x) != TIXML_SUCCESS)
			{
				DIE("ship weapon offset x not found");
			}

			if (weapon->QueryIntAttribute("y", &w_y) != TIXML_SUCCESS)
			{
				DIE("ship weapon offset y not found");
			}
			Weapon* weapon = ship->GetWeapon();
			InitWeapon(w_id, weapon);
			weapon->SetOffset(w_x, w_y);
		}

		spaceships_defs_[id] = ship;
		uniques_.push_back(ship);
		printf("new ship defined: id %d\n", id);
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
		ship->SetTarget(Game::GetInstance().GetPlayerShip());
		return ship;
	}
	DIE("space ship id '%d' is not implemented", id);
	return NULL;
}


Entity* EntityManager::CreateRandomEntity() const
{
	Entity* entity = uniques_[sf::Randomizer::Random(0, uniques_.size() - 1)];
	Entity* copy = entity->Clone();
	return copy;
}


void EntityManager::InitWeapon(int id, Weapon* weapon) const
{
	WeaponMap::const_iterator it = weapon_defs_.find(id);
	if (it != weapon_defs_.end())
	{
		const WeaponDefinition& def = it->second;
		weapon->Init(*def.image, def.fire_rate, def.heat_cost, def.damage,
			def.speed);
		if (!def.sound.empty())
		{
			weapon->SetSoundName(def.sound.c_str());
		}
	}
	else
	{
		DIE("weapon id '%d' is not implemented", id);
	}
}



const Animation& EntityManager::GetAnimation(const char* key) const
{
	AnimationMap::const_iterator it;
	it = animations_.find(key);
	if (it == animations_.end())
	{
		DIE("animation %s not found\n", key);
	}
	return it->second;
}

