#include <stdexcept>
#include <iostream>
#include "EntityManager.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "Spaceship.hpp"
#include "core/LevelManager.hpp"
#include "core/ControlPanel.hpp"
#include "core/ParticleSystem.hpp"
#include "core/MessageSystem.hpp"
#include "core/Resources.hpp"
#include "core/Collisions.hpp"
#include "utils/tinyxml/tinyxml2.h"


/**
 * Get attack pattern encoded in an xml element
 */
static Spaceship::AttackPattern parse_attack_pattern(const tinyxml2::XMLElement* elem)
{
	if (elem->Attribute("attack", "auto_aim")) return Spaceship::AUTO_AIM;
	if (elem->Attribute("attack", "on_sight")) return Spaceship::ON_SIGHT;
	if (elem->Attribute("attack", "none")    ) return Spaceship::NONE;

	if (elem->Attribute("attack") != NULL)
		std::cerr << "unknown attack pattern: " << elem->Attribute("attack") << std::endl;
	return Spaceship::NONE;
}

/**
 * Get movement pattern encoded in an xml element
 */
static Spaceship::MovementPattern parse_movement_pattern(const tinyxml2::XMLElement* elem)
{
	if (elem->Attribute("move", "line")  ) return Spaceship::LINE;
	if (elem->Attribute("move", "magnet")) return Spaceship::MAGNET;
	if (elem->Attribute("move", "sinus") ) return Spaceship::SINUS;
	if (elem->Attribute("move", "circle")) return Spaceship::CIRCLE;

	if (elem->Attribute("move") != NULL)
		std::cerr << "unknown movement pattern: " << elem->Attribute("move") << std::endl;
	return Spaceship::LINE;
}


EntityManager& EntityManager::getInstance()
{
	static EntityManager self;
	return self;
}


EntityManager::EntityManager():
	m_particles(ParticleSystem::getInstance()),
	m_player(NULL),
	m_levels(LevelManager::getInstance())
{
	resize(0, 0);

	more_bad_guys_ = &EntityManager::arcadeModeCallback;
	mode_ = MODE_ARCADE;
	timer_ = 0.f;

	layer1_.m_speed = BACKGROUND_SPEED;
	layer2_.m_speed = FOREGROUND_SPEED;

	decor_height_ = 0;

	// hack, pre-load some resources to avoid in game loading
	Resources::getSoundBuffer("asteroid-break.ogg");
	Resources::getSoundBuffer("door-opening.ogg");
	Resources::getSoundBuffer("boom.ogg");
	Resources::getSoundBuffer("disabled.ogg");
	Resources::getSoundBuffer("overheat.ogg");
	Resources::getSoundBuffer("cooler.ogg");
	Resources::getSoundBuffer("power-up.ogg");
	Resources::getSoundBuffer("ship-damage.ogg");
	Resources::getSoundBuffer("shield-damage.ogg");

	// Init particles emitters
	m_particles.setTexture(&Resources::getTexture("particles/particles.png"));
	m_stars_emitter.setTextureRect(sf::IntRect(32, 9, 3, 3));
	m_stars_emitter.setLifetime(0);
	m_stars_emitter.setSpeed(150, 150);
	m_stars_emitter.setAngle(-math::PI);

	m_impact_emitter.setTextureRect(sf::IntRect(32, 8, 8, 1));
	m_impact_emitter.setScale(1.5, 0.5);
	m_impact_emitter.setSpeed(200, 50);
	m_impact_emitter.setLifetime(3);

	m_green_emitter.setTextureRect(sf::IntRect(40, 8, 8, 8));
}


EntityManager::~EntityManager()
{
	clearEntities();
	for (size_t i = 0; i < uniques_.size(); ++i)
	{
		delete uniques_[i];
	}
}


void EntityManager::InitMode(Mode mode)
{
	// re-init particles
	m_particles.clear();
	MessageSystem::clear();

	ControlPanel::getInstance().Init(mode);

	switch (mode)
	{
		case MODE_STORY:
			ControlPanel::getInstance().SetLevelDuration(m_levels.getDuration());
			more_bad_guys_ = &EntityManager::storyModeCallback;
			// le vaisseau du joueur est conservé d'un niveau à l'autre
			if (mode_ != MODE_STORY || m_player == NULL || m_player->getHP() <= 0)
			{
				RespawnPlayer();
			}
			else
			{
				// Delete all entities but player
				for (EntityList::iterator it = m_entities.begin(); it != m_entities.end();)
				{
					if (*it != m_player)
					{
						delete *it;
						it = m_entities.erase(it);
					}
					else
					{
						++it;
					}
				}
				m_player->setPosition(0, m_height / 2);
				m_player->onInit();
			}
			// Set background images
			if (m_levels.getBottomLayer())
			{
				layer1_.setTexture(*m_levels.getBottomLayer());
				layer1_.setColor(sf::Color::White);
			}

			if (m_levels.getTopLayer())
			{
				layer2_.setTexture(*m_levels.getTopLayer());
				layer2_.setColor(m_levels.getLayerColor());
			}

			decor_height_ = m_levels.getDecorHeight();
			m_stars_emitter.createParticles(m_levels.getStarsCount());

			// Set background music
			if (m_levels.getMusicName() != NULL)
			{
				std::string filename = Resources::getSearchPath() + "/music/" + m_levels.getMusicName();
				SoundSystem::openMusicFromFile(filename);
				SoundSystem::playMusic();
			}
			else
			{
				SoundSystem::stopMusic();
			}
			break;

		case MODE_ARCADE:
		{
			more_bad_guys_ = &EntityManager::arcadeModeCallback;
			// on démarre toujours le mode arcade avec un nouveau vaisseau
			RespawnPlayer();
			// fog with random color on layer 2,
			sf::Color color(math::rand(0, 40), math::rand(0, 40), math::rand(20, 60));
			layer1_.setTexture(Resources::getTexture("layers/blue.jpg"));
			layer1_.setColor(color);
			layer2_.setTexture(Resources::getTexture("layers/fog.png"));
			layer2_.setColor(color);

			decor_height_ = 0;
			m_stars_emitter.createParticles(math::rand(50, 100));
			SoundSystem::openMusicFromFile(Resources::getSearchPath() + "/music/spacesong.mod");
			SoundSystem::playMusic();

			max_droppable_index_ = 1;
			max_droppable_points_ = 0;
			break;
		}
	}
	mode_ = mode;
	timer_ = 0.f;
}


EntityManager::Mode EntityManager::GetMode() const
{
	return mode_;
}


void EntityManager::resize(int width, int height)
{
	m_width = std::max(0, width);
	m_height = std::max(0, height);
}


void EntityManager::update(float frametime)
{
	EntityList::iterator it, it2;

	sf::FloatRect r1, r2;
	for (it = m_entities.begin(); it != m_entities.end();)
	{
		Entity& entity = **it;
		entity.onUpdate(frametime);
		Entity::BoundingBox box = entity.getBoundingBox();

		if (entity.isDead())
		{
			// Remove dead entities
			delete *it;
			it = m_entities.erase(it);
		}
		else if (box.right < 0 || box.bottom < 0 || box.left > m_width || box.top > m_height)
		{
			// Remove entities outside the entity manager
			delete *it;
			it = m_entities.erase(it);
		}
		else
		{
			it2 = it;
			for (++it2; it2 != m_entities.end(); ++it2)
			{
				// Collision dectection it1 <-> it2
				if (Collisions::pixelPerfectTest(entity, **it2))
				{
					entity.collides(**it2);
					(**it2).collides(entity);
				}
			}
			++it;
		}
	}

	// Update and collision
	if (decor_height_ > 0)
	{
		// FIXME: decor height applies only on player
		float player_y = m_player->getY();
		if (player_y < decor_height_)
		{
			m_player->setY(decor_height_ + 1);
			m_player->takeDamage(1);
		}
		else if ((player_y + m_player->getHeight()) > (m_height - decor_height_))
		{
			m_player->setY(m_height - decor_height_ - m_player->getHeight() - 1);
			m_player->takeDamage(1);
		}
	}

	m_particles.update(frametime);
	MessageSystem::update(frametime);

	// parallax scrolling
	layer1_.scroll(frametime);
	layer2_.scroll(frametime);

	timer_ += frametime;
}


void EntityManager::addEntity(Entity* entity)
{
	entity->onInit();
	m_entities.push_back(entity);
}


void EntityManager::clearEntities()
{
	for (EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		delete *it;
	}
	m_entities.clear();
}


size_t EntityManager::count() const
{
	return m_entities.size();
}


bool EntityManager::spawnBadGuys()
{
	return !(this->*more_bad_guys_)() || m_player == NULL || m_player->isDead();
}


void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	// Draw background layers
	target.draw(layer1_, states);
	target.draw(layer2_, states);

	// Draw effects
	target.draw(m_particles, states);
	MessageSystem::show(target, states);

	// Draw managed entities
	for (EntityList::const_iterator it = m_entities.begin(); it != m_entities.end(); ++it)
	{
		target.draw(**it, states);
	}
}


void EntityManager::loadAnimations(const std::string& filename)
{
	// Open XML file
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != 0)
	{
		std::string error = "Cannot load animations from " + filename + ": " + doc.GetErrorStr1();
		throw std::runtime_error(error);
	}

	tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement();
	while (elem != NULL)
	{
		// Parse attributes
		bool ok = true;
		const char* name;
		ok &= ((name = elem->Attribute("name")) != NULL);
		const char* img;
		ok &= ((img = elem->Attribute("img")) != NULL);
		int width, height, count;
		ok &= (elem->QueryIntAttribute("width", &width) == tinyxml2::XML_SUCCESS);
		ok &= (elem->QueryIntAttribute("height", &height) == tinyxml2::XML_SUCCESS);
		ok &= (elem->QueryIntAttribute("count", &count) == tinyxml2::XML_SUCCESS);
		float delay = 0.f;
		ok &= (elem->QueryFloatAttribute("delay", &delay) == tinyxml2::XML_SUCCESS);
		bool reverse = false;
		elem->QueryBoolAttribute("reverse", &reverse);
		int x = 0, y = 0;
		elem->QueryIntAttribute("x", &x);
		elem->QueryIntAttribute("y", &y);

		if (ok)
		{
			// Add each frame in the animation
			Animation* anim = &animations_[name];
			for (int i = 0; i < count; ++i)
				anim->addFrame({x + i * width, y, width, height});

			if (reverse)
			{
				// Loop back to the begining
				for (int i = count - 2; i >= 0; --i)
					anim->addFrame({x + i * width, y, width, height});
			}
			anim->setDelay(delay);
			anim->setTexture(Resources::getTexture(img));
			Collisions::registerTexture(&anim->getTexture());
		}
		elem = elem->NextSiblingElement();
	}
}


void EntityManager::loadSpaceships(const std::string& filename)
{
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename.c_str()) != 0)
	{
		std::string error = "Cannot load spaceships from " + filename + ": " + doc.GetErrorStr1();
		throw std::runtime_error(error);
	}

	tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement();
	while (elem != NULL)
	{
		const char* id = elem->Attribute("id");
		if (id == NULL)
			throw std::runtime_error("XML parse error: spaceship.id is missing");

		const char* animation = elem->Attribute("animation");
		if (animation == NULL)
			throw std::runtime_error("XML parse error: spaceship.animation is missing");

		int hp = 0;
		if (elem->QueryIntAttribute("hp", &hp) != tinyxml2::XML_SUCCESS)
			throw std::runtime_error("XML parse error: spaceship.hp is missing");

		int speed = 0;
		if (elem->QueryIntAttribute("speed", &speed) != tinyxml2::XML_SUCCESS)
			throw std::runtime_error("XML parse error: spaceship.speed is missing");

		int points = 0;
		elem->QueryIntAttribute("points", &points);

		// Create spaceship instance
		Spaceship* ship = new Spaceship(getAnimation(animation), hp, speed);
		ship->setPoints(points);
		ship->setMovementPattern(parse_movement_pattern(elem));
		ship->setAttackPattern(parse_attack_pattern(elem));

		// Parse weapon tag
		tinyxml2::XMLElement* weapon = elem->FirstChildElement();
		if (weapon != NULL)
		{
			int w_x, w_y;
			const char* weapon_id = weapon->Attribute("id");
			if (weapon_id == NULL)
				throw std::runtime_error("XML parse error: spaceship.weapon.id is missing");

			if (weapon->QueryIntAttribute("x", &w_x) != tinyxml2::XML_SUCCESS)
				throw std::runtime_error("XML parse error: spaceship.weapon.x is missing");

			if (weapon->QueryIntAttribute("y", &w_y) != tinyxml2::XML_SUCCESS)
				throw std::runtime_error("XML parse error: spaceship.weapon.y is missing");

			ship->getWeapon().init(weapon_id);
			ship->getWeapon().setPosition({(float) w_x, (float) w_y});
		}
#ifdef DEBUG
		std::cout << "\t" << elem->Attribute("name") << " " << id << std::endl;
#endif
		m_spaceships[id] = ship;
		RegisterUniqueEntity(ship);
		elem = elem->NextSiblingElement();
	}
}


Spaceship* EntityManager::createSpaceship(const std::string& id) const
{
	SpaceshipMap::const_iterator it = m_spaceships.find(id);
	if (it != m_spaceships.end())
	{
		return it->second->clone();
	}
	std::cerr << "Couldn't create spaceship width id: " << id << std::endl;
	return NULL;
}


const Animation& EntityManager::getAnimation(const std::string& key) const
{
	AnimationMap::const_iterator it;
	it = animations_.find(key);
	if (it == animations_.end())
	{
		std::string err = "Animation '" + key + "' doesn't exist";
		throw std::runtime_error(err);
	}
	return it->second;
}


Player* EntityManager::getPlayer() const
{
	assert(m_player != NULL);
	return m_player;
}


Entity* EntityManager::createRandomEntity()
{

	if (math::rand(0, 9) == 0)
	{
		// Spawn asteroid
		return new Asteroid(Asteroid::BIG);
	}
	else
	{
		// Progressive difficulty: the longer elapsed time is, the strongest entities are
		// DROP_DELAY_STEP is time to wait before an entity type is able to be spawned

		// Entities points:    Cannot appear before (seconds):
		// 1 point             0
		// 2 points            DROP_DELAY_STEP
		// 3 points            DROP_DELAY_STEP * 2
		// 4 points            DROP_DELAY_STEP * 3
		// etc.

		static const int DROP_DELAY_STEP = 25;

		// Update index of the strongest spawnable spaceship
		max_droppable_points_ = timer_ / DROP_DELAY_STEP + 1;

		for (size_t i = max_droppable_index_; i < uniques_.size(); ++i)
		{
			Spaceship* spaceship = uniques_[i];
			if (spaceship->getPoints() <= max_droppable_points_)
			{
				max_droppable_index_ = i;
			}
			else
			{
				break;
			}
		}
		return uniques_[math::rand(0, max_droppable_index_)]->clone();
	}
}


void EntityManager::RegisterUniqueEntity(Spaceship* entity)
{
	std::vector<Spaceship*>::iterator it = uniques_.begin();
	while (it != uniques_.end() && (**it).getPoints() < entity->getPoints())
	{
		++it;
	}
	uniques_.insert(it, entity);
}


bool EntityManager::arcadeModeCallback()
{
	// Number of max entities = time / STEP + START
	const int STEP = 8;
	const int START = 1;
	if (count() < timer_ / STEP + START)
	{
		Entity* entity = createRandomEntity();
		entity->setX(m_width - 1);
		entity->setY(math::rand(0, m_height - (int) entity->getHeight()));
		entity->move(entity->getOrigin());
		addEntity(entity);
	}
	// Always true, spawn infinite entities, player will die eventually... :o)
	return true;
}


bool EntityManager::storyModeCallback()
{
	// Move entities from the spawn queue to the EntityManager
	Entity* next = m_levels.spawnNextEntity(timer_);
	while (next != NULL)
	{
		next->move(next->getOrigin());
		addEntity(next);
		next = m_levels.spawnNextEntity(timer_);
	}

	// The current level is completed when there is no remaining entities in the
	// LevelManager's spawn queue and Player is the only entity still active
	return m_levels.getSpawnQueueSize() > 0 || count() > 1;
}


void EntityManager::RespawnPlayer()
{
	clearEntities();
	m_player = new Player();
	m_player->setPosition(50, m_height / 2);
	addEntity(m_player);
}


void EntityManager::createImpactParticles(const sf::Vector2f& pos, size_t count)
{
	m_impact_emitter.setPosition(pos);
	m_impact_emitter.createParticles(count);
}


void EntityManager::createGreenParticles(const sf::Vector2f& pos, size_t count)
{
	m_green_emitter.setPosition(pos);
	m_green_emitter.createParticles(count);
}

// StarsEmitter ----------------------------------------------------------------

void EntityManager::StarsEmitter::onParticleCreated(ParticleSystem::Particle& particle) const
{
	particle.position = sf::Vector2f(math::rand(0, EntityManager::getInstance().getWidth()),
	                                 math::rand(0, EntityManager::getInstance().getHeight()));
}


void EntityManager::StarsEmitter::onParticleUpdated(ParticleSystem::Particle& particle, float) const
{
	if (particle.position.x < 0)
	{
		resetParticle(particle);
		particle.position.x = EntityManager::getInstance().getWidth();
	}
}

// ParallaxLayer ---------------------------------------------------------------

EntityManager::ParallaxLayer::ParallaxLayer()
{
	m_speed = 0.f;
	m_background.setPosition(0.f, 0.f);
	m_background2.setPosition(0.f, 0.f);
	m_blend_mode = sf::BlendAlpha;
}


void EntityManager::ParallaxLayer::scroll(float frametime)
{
	float x = m_background.getPosition().x - m_speed * frametime;
	float width = m_background.getTextureRect().width;
	if (x <= -width)
	{
		x = 0.f;
	}
	m_background.setPosition(x, 0.f);
	m_background2.setPosition(x + width, 0.f);
}


void EntityManager::ParallaxLayer::setTexture(const sf::Texture& texture)
{
	m_background.setTexture(texture, true);
	m_background.setPosition(0.f, 0.f);
	m_background2.setTexture(texture, true);
	m_background2.setPosition(texture.getSize().x, 0.f);
}


void EntityManager::ParallaxLayer::setColor(const sf::Color& color)
{
	if (color != sf::Color::White)
	{
		m_background.setColor(color);
		m_background2.setColor(color);
		m_blend_mode = sf::BlendAdd;
	}
	else
	{
		m_background.setColor(sf::Color::White);
		m_background2.setColor(sf::Color::White);
		m_blend_mode = sf::BlendAlpha;
	}
}


void EntityManager::ParallaxLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.blendMode = m_blend_mode;
	target.draw(m_background, states);
	target.draw(m_background2, states);
}
