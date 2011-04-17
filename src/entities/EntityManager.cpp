#include "EntityManager.hpp"
#include "Asteroid.hpp"
#include "PlayerShip.hpp"

#include "../core/Game.hpp"
#include "../core/LevelManager.hpp"
#include "../core/ControlPanel.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/DIE.hpp"
#include "../utils/I18n.hpp"
#include "../tinyxml/tinyxml.h"


EntityManager& EntityManager::GetInstance()
{
	static EntityManager self;
	return self;
}


EntityManager::EntityManager():
	particles_(ParticleSystem::GetInstance()),
	levels_(LevelManager::GetInstance())
{
	SetSize(Game::WIDTH, Game::HEIGHT);

	player_ = NULL;
	more_bad_guys_ = &EntityManager::MoreBadGuys_ARCADE;
	game_over_ = false;
	mode_ = MODE_ARCADE;
	timer_ = arcade_record_ = 0.f;
}


EntityManager::~EntityManager()
{
	Clear();
	for (size_t i = 0; i < uniques_.size(); ++i)
	{
		delete uniques_[i];
	}
}


void EntityManager::InitMode(Mode mode)
{
	// re-init particles
	particles_.Clear();

	switch (mode)
	{
		case MODE_STORY:
			more_bad_guys_ = &EntityManager::MoreBadGuys_STORY;
			// le vaisseau du joueur est conservé d'un niveau à l'autre
			if (mode_ != MODE_STORY || player_ == NULL || player_->IsDead())
			{
				RespawnPlayer();
			}
			else
			{
				// suppression de toutes les unités, sauf le joueur
				for (EntityList::iterator it = entities_.begin();
					it != entities_.end();)
				{
					if (*it != player_)
					{
						delete *it;
						it = entities_.erase(it);
					}
					else
					{
						++it;
					}
				}
				player_->SetPosition(0, height_ / 2);
			}
			particles_.AddStars(LevelManager::GetInstance().GetStarsCount());
			break;

		case MODE_ARCADE:
			more_bad_guys_ = &EntityManager::MoreBadGuys_ARCADE;
			// on démarre toujours le mode arcade avec un nouveau vaisseau
			RespawnPlayer();
			SetBackgroundColor(sf::Color::Black, sf::Color::Black);
			ControlPanel::GetInstance().SetGameInfo(
				str_sprintf(I18n::t("panel.record").c_str(),
				(int) arcade_record_ / 60,
				(int) arcade_record_ % 60));
			particles_.AddStars();
			break;
	}
	mode_ = mode;
	ControlPanel::GetInstance().Init(mode);
	ControlPanel::GetInstance().SetLevelDuration(levels_.GetDuration());
	// initialisation avant une nouvelle partie
	game_over_ = false;
	timer_ = 0.f;
	particles_.AddShield(player_->GetShield(), player_);
}


EntityManager::Mode EntityManager::GetMode() const
{
	return mode_;
}


void EntityManager::SetSize(int width, int height)
{
	if (width < 0)
	{
		width = 0;
	}
	if (height < 0)
	{
		height = 0;
	}
	background_ = sf::Shape::Rectangle(0, 0, width, height, sf::Color(255, 255, 255, 0));
	background_.SetColor(sf::Color::White);
	width_ = width;
	height_ = height;
}


int EntityManager::GetWidth() const
{
	return width_;
}


int EntityManager::GetHeight() const
{
	return height_;
}


void EntityManager::HandleAction(Input::Action action)
{
	player_->HandleAction(action);
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
	for (it = entities_.begin(); it != entities_.end(); ++it)
	{
		(**it).Update(frametime);

		it2 = it;
		for (++it2; it2 != entities_.end(); ++it2)
		{
			if ((**it).IsCollidingWith(**it2))
			{
				(**it).OnCollide(**it2);
				(**it2).OnCollide(**it);
			}
		}
	}
	particles_.Update(frametime);
	timer_ += frametime;
}


void EntityManager::AddEntity(Entity* entity)
{
	entity->SetTarget(player_);
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
	return (int) entities_.size();
}


void EntityManager::TerminateGame()
{
	game_over_ = true;
}


bool EntityManager::CheckGameOver()
{
	return (this->*more_bad_guys_)() || game_over_;
}


void EntityManager::SetBackgroundColor(const sf::Color& top, const sf::Color& bottom)
{
	background_.SetPointColor(0, top);    // top left
	background_.SetPointColor(1, top);    // top right
	background_.SetPointColor(2, bottom); // bottom left
	background_.SetPointColor(3, bottom); // bottom right
}


void EntityManager::UpdateArcadeRecord()
{
	assert(timer_ > arcade_record_);
	arcade_record_ = timer_;
}


void EntityManager::Render(sf::RenderTarget& target) const
{
	target.Draw(background_);
	particles_.Show(target);
	// draw each managed entity
	for (EntityList::const_iterator it = entities_.begin(); it != entities_.end(); ++it)
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
	puts("* loading weapons...");
	TiXmlElement* elem = doc.RootElement()->FirstChildElement();
	while (elem != NULL)
	{
		const char* p = elem->Attribute("id");
		if (p == NULL)
		{
			DIE("weapon id is missing");
		}
		WeaponDefinition* weapon = &weapon_defs_[p];

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
	// chargement des animations
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		DIE("can't open animation definitions: %s (%s)", filename, doc.ErrorDesc());
	}
	puts("* loading animations...");
	TiXmlElement* elem = doc.RootElement()->FirstChildElement();
	// attributs
	int width, height, count;
	float delay = 0.f;
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
		}
		elem = elem->NextSiblingElement();
	}
}


void EntityManager::LoadSpaceShips(const char* filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		DIE("can't load space ships definitions: '%s' (%s)", filename, doc.ErrorDesc());
	}
	puts("* loading spaceships...");
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
			int w_x, w_y;
			const char* weapon_id = weapon->Attribute("id");
			if (weapon_id == NULL)
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
			ship->GetWeapon()->Init(weapon_id);
			ship->GetWeapon()->SetOffset(w_x, w_y);
		}

		spaceships_defs_[id] = ship;
		uniques_.push_back(ship);
		elem = elem->NextSiblingElement();
	}

	uniques_.push_back(new Asteroid(sf::Vector2f(0, 0), Asteroid::BIG));
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
	DIE("space ship id '%d' is not implemented", id);
	return NULL;
}


Entity* EntityManager::CreateRandomEntity() const
{
	Entity* entity = uniques_[sf::Randomizer::Random(0, uniques_.size() - 1)];
	return entity->Clone();
}


void EntityManager::InitWeapon(const char* id, Weapon* weapon) const
{
	WeaponMap::const_iterator it = weapon_defs_.find(id);
	if (it != weapon_defs_.end())
	{
		const WeaponDefinition& def = it->second;
		weapon->SetImage(def.image);
		weapon->SetFireRate(def.fire_rate);
		weapon->SetHeatCost(def.heat_cost);
		weapon->SetDamage(def.damage);
		weapon->SetVelociy(def.speed);
		if (!def.sound.empty())
		{
			weapon->SetSoundName(def.sound.c_str());
		}
	}
	else
	{
		DIE("weapon id '%s' is not implemented", id);
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


Entity* EntityManager::GetPlayerShip() const
{
	assert(player_ != NULL);
	return player_;
}


bool EntityManager::MoreBadGuys_ARCADE()
{
	// number of max bad guys = time / STEP + START
	const int STEP = 8;
	const int START = 1;
	if (Count() < timer_ / STEP + START)
	{
		Entity* entity = CreateRandomEntity();
		sf::Vector2f pos;
		pos.x = Game::WIDTH;
		pos.y = sf::Randomizer::Random(0,
			height_ - (int) entity->GetSize().y);
		entity->SetPosition(pos);
		AddEntity(entity);
	}
	// always false, spawn bad guys till you die
	return false;
}


bool EntityManager::MoreBadGuys_STORY()
{
	Entity* p = levels_.GiveNextEntity(timer_);
	while (p != NULL)
	{
		AddEntity(p);
		p = levels_.GiveNextEntity(timer_);
	}

	// le niveau n'est pas fini tant qu'il reste des ennemis, soit en file
	// d'attente, soit dans le gestionnaire d'entités
	return levels_.RemainingEntities() == 0 && Count() == 1; // 0 ennemis + le joueur = 1
}


void EntityManager::RespawnPlayer()
{
	Clear();
	sf::Vector2f position(50, height_ / 2);
	player_ = new PlayerShip(position, "player");
	AddEntity(player_);
}
