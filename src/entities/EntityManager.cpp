#include "EntityManager.hpp"
#include "Asteroid.hpp"
#include "PlayerShip.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "utils/Resources.hpp"
#include "utils/StringUtils.hpp"
#include "utils/Error.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"
#include "tinyxml/tinyxml.h"

/*
En mode arcade, la difficulté est progressive
Cette constante est le temps, en secondes, avant l'apparition d'entitées de valeur max N + 1

valeur max :  	apparait à partir de :
1 point       	0
2 points      	DROPPABLE_STEP
3 points      	DROPPABLE_STEP * 2
4 points      	DROPPABLE_STEP * 3
...
*/
#define DROPPABLE_STEP 25


EntityManager& EntityManager::GetInstance()
{
	static EntityManager self;
	return self;
}


EntityManager::EntityManager():
	particles_(ParticleSystem::GetInstance()),
	levels_(LevelManager::GetInstance())
{
	SetSize(0, 0);

	player_ = NULL;
	more_bad_guys_ = &EntityManager::MoreBadGuys_ARCADE;
	game_over_ = false;
	mode_ = MODE_ARCADE;
	timer_ = arcade_record_ = 0.f;

	layer1_.scrolling_speed_ = BACKGROUND_SPEED;
	layer2_.scrolling_speed_ = FOREGROUND_SPEED;

	decor_height_ = 0;
	SetY(ControlPanel::HEIGHT); // default positon
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

	ControlPanel::GetInstance().Init(mode);
	LevelManager& levels = LevelManager::GetInstance();

	switch (mode)
	{
		case MODE_STORY:
			ControlPanel::GetInstance().SetLevelDuration(levels_.GetDuration());
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
				player_->Initialize();
			}
			layer1_.SetScrollingTexture(levels.GetLayerImage1());
			layer2_.SetScrollingTexture(levels.GetLayerImage2());
			layer2_.SetColor(levels.GetLayerColor());
			decor_height_ = levels.GetDecorHeight();
			particles_.AddStars(levels.GetStarsCount());
			break;

		case MODE_ARCADE:
			more_bad_guys_ = &EntityManager::MoreBadGuys_ARCADE;
			// on démarre toujours le mode arcade avec un nouveau vaisseau
			RespawnPlayer();
			// no image on layer 1, fog with random color on layer 2,
			layer1_.SetScrollingTexture(NULL);
			layer2_.SetScrollingTexture(&Resources::GetImage("layers/fog.png"));
			layer2_.SetColor(math::random_color(0, 0, 20, 30, 30, 70));
			decor_height_ = 0;
			std::wstring game_info = wstr_replace(_t("panel.record"), L"{record}", to_wstring(arcade_record_));
			ControlPanel::GetInstance().SetGameInfo(game_info);
			particles_.AddStars();

			max_droppable_index_ = 1;
			max_droppable_points_ = 0;
			break;
	}
	mode_ = mode;

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
	width_ = width;
	height_ = height;
}


void EntityManager::HandleAction(Input::Action action)
{
	player_->HandleAction(action);
}


void EntityManager::Update(float frametime)
{
	EntityList::iterator it, it2;

	sf::FloatRect r1, r2;
	for (it = entities_.begin(); it != entities_.end();)
	{
		Entity& entity = **it;
		entity.Update(frametime);

		r1 = entity.GetCollideRect();

		if (entity.IsDead())
		{
			// removing dead entities
			delete *it;
			it = entities_.erase(it);
		}
		else if (r1.Bottom < 0 || r1.Top > height_ || r1.Right < 0 || r1.Left > width_)
		{
			// removing entities outside the entity manager
			delete *it;
			it = entities_.erase(it);
		}
		else
		{
			it2 = it;
			for (++it2; it2 != entities_.end(); ++it2)
			{
				// collision dectection it1 <-> it2
				r2 = (**it2).GetCollideRect();
				if (entity.IsCollidingWith(**it2, r1, r2))
				{
					entity.OnCollide(**it2);
					(**it2).OnCollide(entity);
				}
			}
			++it;
		}

	}

	// update and collision
	if (decor_height_ > 0)
	{
		// decor height applies only on player
		float player_y = player_->GetPosition().y;
		if (player_y < decor_height_)
		{
			player_->SetY(decor_height_ + 1);
			player_->TakeDamage(1);
		}
		else if ((player_y + player_->GetSize().y) > (height_ - decor_height_))
		{
			player_->SetY(height_ - decor_height_ - player_->GetSize().y - 1);
			player_->TakeDamage(1);
		}
	}

	particles_.Update(frametime);

	// parallax scrolling
	layer1_.OnUpdate(frametime);
	layer2_.OnUpdate(frametime);

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


bool EntityManager::IsGameOver()
{
	return (this->*more_bad_guys_)() || game_over_;
}


void EntityManager::UpdateArcadeRecord()
{
	assert(player_->GetPoints() > arcade_record_);
	arcade_record_ = player_->GetPoints();
}


void EntityManager::Render(sf::RenderTarget& target) const
{
	// draw scrolling background images
	layer1_.Draw(target);
	layer2_.Draw(target);
	particles_.Show(target);

	// draw each managed entity
	for (EntityList::const_iterator it = entities_.begin(); it != entities_.end(); ++it)
	{
		target.Draw(**it);
	}
}


int EntityManager::LoadAnimations(const std::string& filename)
{
	// chargement des animations
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		Error::Log << "Cannot load animations definition:\n" << doc.ErrorDesc() << "\n";
		throw Error::Exception();
	}

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
			p->SetImage(Resources::GetImage(img));
		}
		elem = elem->NextSiblingElement();
	}
	return animations_.size();
}


int EntityManager::LoadSpaceShips(const std::string& filename)
{
	TiXmlDocument doc;
	if (!doc.LoadFile(filename))
	{
		Error::Log << "Cannot load spaceships definition:\n" << doc.ErrorDesc();
		throw Error::Exception();
	}

	TiXmlElement* elem = doc.RootElement()->FirstChildElement();
	while (elem != NULL)
	{
		int id;
		if (elem->QueryIntAttribute("id", &id) != TIXML_SUCCESS)
			throw Error::Exception("parse error: spaceship[id] is missing");

		const char* name = elem->Attribute("name");
		if (name == NULL)
			throw Error::Exception("parse error: spaceship[name] is missing");

		const char* animation = elem->Attribute("animation");
		if (animation == NULL)
			throw Error::Exception("parse error: spaceship[animation] is missing");

		int hp;
		if (elem->QueryIntAttribute("hp", &hp) != TIXML_SUCCESS)
			throw Error::Exception("parse error: spaceship[hp] is missing");

		int speed;
		if (elem->QueryIntAttribute("speed", &speed) != TIXML_SUCCESS)
			throw Error::Exception("parse error: spaceship[speed] is missing");

		int points = 0;
		elem->QueryIntAttribute("points", &points);


		SpaceShip* ship = new SpaceShip(GetAnimation(animation), hp, speed);
		ship->SetPoints(points);

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
				throw Error::Exception("parse error: spaceship > weapon[id] is missing");

			if (weapon->QueryIntAttribute("x", &w_x) != TIXML_SUCCESS)
				throw Error::Exception("parese error: spaceship > weapon[x] is missing");

			if (weapon->QueryIntAttribute("y", &w_y) != TIXML_SUCCESS)
				throw Error::Exception("parese error: spaceship > weapon[y] is missing");

			ship->GetWeapon()->Init(weapon_id);
			ship->GetWeapon()->SetOffset(w_x, w_y);
		}

		spaceships_defs_[id] = ship;
		RegisterUniqueEntity(ship);
		elem = elem->NextSiblingElement();
	}

	RegisterUniqueEntity(new Asteroid(sf::Vector2f(0, 0), Asteroid::BIG));
	return spaceships_defs_.size();
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
	std::cerr << "spaceship id:" << id << " is not defined" << std::endl;
	return NULL;
}


const Animation& EntityManager::GetAnimation(const std::string& key) const
{
	AnimationMap::const_iterator it;
	it = animations_.find(key);
	if (it == animations_.end())
	{
		Error::Log << "Animation " << key << " doesn't exist\n";
		throw Error::Exception();
	}
	return it->second;
}


PlayerShip* EntityManager::GetPlayerShip() const
{
	assert(player_ != NULL);
	return player_;
}


void EntityManager::SpawnRandomEntity()
{
	// update index of the most valuable spwanable entity
	max_droppable_points_ = timer_ / DROPPABLE_STEP + 1;

	for (size_t i = max_droppable_index_; i < uniques_.size(); ++i)
	{
		Entity* entity = uniques_[i];
		if (entity->GetPoints() <= max_droppable_points_)
		{
			max_droppable_index_ = i;
		}
		else
		{
			break;
		}
	}
	Entity* entity = uniques_[sf::Randomizer::Random(0, max_droppable_index_)]->Clone();

	entity->SetX(width_ - 1);
	entity->SetY(sf::Randomizer::Random(0, height_ - (int) entity->GetSize().y));
	AddEntity(entity);
}


void EntityManager::RegisterUniqueEntity(Entity* entity)
{
	std::vector<Entity*>::iterator it = uniques_.begin();
	while (it != uniques_.end() && (**it).GetPoints() < entity->GetPoints())
	{
		++it;
	}
	uniques_.insert(it, entity);
}


bool EntityManager::MoreBadGuys_ARCADE()
{
	// number of max bad guys = time / STEP + START
	const int STEP = 8;
	const int START = 1;
	if (Count() < timer_ / STEP + START)
	{
		SpawnRandomEntity();
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



EntityManager::ParallaxLayer::ParallaxLayer()
{
	image_ = NULL;
	scrolling_speed_ = 0.f;
	background_.SetPosition(0.f, 0.f);
	background2_.SetPosition(0.f, 0.f);
}


void EntityManager::ParallaxLayer::OnUpdate(float frametime)
{
	if (image_ != NULL)
	{
		float x = background_.GetPosition().x - scrolling_speed_ * frametime;
		float width = image_->GetWidth();
		if (x <= -width)
		{
			x = 0;
		}
		background_.SetX(x);
		background2_.SetX(x + width);
	}
}


void EntityManager::ParallaxLayer::SetScrollingTexture(const sf::Image* image)
{
	image_ = image;
	if (image != NULL)
	{
		background_ = sf::Sprite(*image);
		background2_ = sf::Sprite(*image);
		background2_.SetX(image->GetWidth());
	}
}


void EntityManager::ParallaxLayer::SetColor(const sf::Color& color)
{
	if (color != sf::Color::White)
	{
		background_.SetColor(color);
		background2_.SetColor(color);
		background_.SetBlendMode(sf::Blend::Add);
		background2_.SetBlendMode(sf::Blend::Add);
	}
	else
	{
		background_.SetColor(sf::Color::White);
		background2_.SetColor(sf::Color::White);
		background_.SetBlendMode(sf::Blend::Alpha);
		background2_.SetBlendMode(sf::Blend::Alpha);
	}
}

void EntityManager::ParallaxLayer::Draw(sf::RenderTarget& target) const
{
	if (image_ != NULL)
	{
		target.Draw(background_);
		target.Draw(background2_);
	}
}
