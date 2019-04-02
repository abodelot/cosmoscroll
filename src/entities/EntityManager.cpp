#include <stdexcept>
#include <iostream>
#include <cassert>
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
#include "vendor/tinyxml/tinyxml2.h"


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
    m_spawnMethod(&EntityManager::infinityModeCallback),
    m_mode(INFINITY_MODE),
    m_timer(0),
    m_player(NULL),
    m_width(0),
    m_height(0),
    m_decor_height(0),
    m_levels(LevelManager::getInstance()),
    m_particles(ParticleSystem::getInstance())
{
    // HACK: pre-load some resources to avoid in game loading
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
}


void EntityManager::setMode(Mode mode)
{
    // re-init particles
    m_particles.clear();
    MessageSystem::clear();

    ControlPanel::getInstance().setMode(mode);

    switch (mode)
    {
        case LEVELS_MODE:
            ControlPanel::getInstance().setLevelDuration(m_levels.getDuration());
            m_spawnMethod = &EntityManager::levelsCallback;
            // le vaisseau du joueur est conservé d'un niveau à l'autre
            if (m_mode != LEVELS_MODE || m_player == NULL || m_player->getHP() <= 0)
            {
                respawnPlayer();
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
                m_layer1.setTexture(*m_levels.getBottomLayer());
                m_layer1.setColor(sf::Color::White);
            }

            if (m_levels.getTopLayer())
            {
                m_layer2.setTexture(*m_levels.getTopLayer());
                m_layer2.setColor(m_levels.getLayerColor());
            }

            m_decor_height = m_levels.getDecorHeight();
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

        case INFINITY_MODE:
        {
            m_spawnMethod = &EntityManager::infinityModeCallback;
            respawnPlayer();

            // Fog with random color on layer 2,
            sf::Color color(math::rand(0, 40), math::rand(0, 40), math::rand(20, 60));
            m_layer1.setTexture(Resources::getTexture("layers/blue.jpg"));
            m_layer1.setColor(color);
            m_layer2.setTexture(Resources::getTexture("layers/fog.png"));
            m_layer2.setColor(color);

            m_decor_height = 0;
            m_stars_emitter.createParticles(math::rand(50, 100));
            SoundSystem::openMusicFromFile(Resources::getSearchPath() + "/music/spacesong.mod");
            SoundSystem::playMusic();
            m_max_droppable_index = 1;
            m_max_droppable_points = 0;
            break;
        }
    }
    m_mode = mode;
    m_timer = 0.f;
}


EntityManager::Mode EntityManager::getMode() const
{
    return m_mode;
}


void EntityManager::resize(int width, int height)
{
    m_width = std::max(0, width);
    m_height = std::max(0, height);
}


void EntityManager::update(float frametime)
{
    EntityList::iterator it, it2;

    // Update and collision
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

    // HACK: decor height applies only on player
    if (m_decor_height > 0)
    {
        float player_y = m_player->getY();
        if (player_y < m_decor_height)
        {
            m_player->setY(m_decor_height + 1);
            m_player->takeDamage(1);
        }
        else if ((player_y + m_player->getHeight()) > (m_height - m_decor_height))
        {
            m_player->setY(m_height - m_decor_height - m_player->getHeight() - 1);
            m_player->takeDamage(1);
        }
    }

    m_particles.update(frametime);
    MessageSystem::update(frametime);

    // Parallax scrolling
    m_layer1.scroll(BACKGROUND_SPEED * frametime);
    m_layer2.scroll(FOREGROUND_SPEED * frametime);

    m_timer += frametime;
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


bool EntityManager::spawnBadGuys()
{
    return !(this->*m_spawnMethod)() || m_player == NULL || m_player->isDead();
}


void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    // Draw background layers
    target.draw(m_layer1, states);
    target.draw(m_layer2, states);

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
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0)
    {
        std::string error = "Cannot load animations from " + filename + ": " + doc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement("anim");
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

        int x = 0, y = 0;
        elem->QueryIntAttribute("x", &x);
        elem->QueryIntAttribute("y", &y);

        if (ok)
        {
            // Create animation and add frames
            Animation& animation = m_animations[name];
            for (int i = 0; i < count; ++i)
                animation.addFrame({x + i * width, y, width, height});

            animation.setDelay(delay);
            animation.setTexture(Resources::getTexture(img));
            Collisions::registerTexture(&animation.getTexture());
        }
        elem = elem->NextSiblingElement("anim");
    }
}


const Animation& EntityManager::getAnimation(const std::string& id) const
{
    AnimationMap::const_iterator it = m_animations.find(id);
    if (it == m_animations.end())
        throw std::runtime_error("Animation '" + id + "' not found");

    return it->second;
}


void EntityManager::loadWeapons(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0)
    {
        std::string error = "Cannot load weapons from " + filename + ": " + doc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    // Loop over 'weapon' tags
    tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement("weapon");
    while (elem != NULL)
    {
        // Weapon ID
        const char* p = elem->Attribute("id");
        if (!p)
            throw std::runtime_error("XML error: weapon.id is missing");

        Weapon& weapon = m_weapons[p];

        p = elem->Attribute("image"); // Projectile image
        if (p)
            weapon.setTexture(&Resources::getTexture(p));
        else
            std::cerr << "XML error: weapon.image is missing" << std::endl;

        p = elem->Attribute("sound"); // Sound effect
        if (p)
            weapon.setSound(&Resources::getSoundBuffer(p));
        else
            std::cerr << "XML error: weapon.sound is missing" << std::endl;

        float heatcost = 0.f;
        if (elem->QueryFloatAttribute("heatcost", &heatcost) == 0)
            weapon.setHeatCost(heatcost);
        else
            std::cerr << "XML error: weapon.heatcost is missing" << std::endl;

        float firerate = 0.f;
        if (elem->QueryFloatAttribute("firerate", &firerate) == 0)
            weapon.setFireRate(firerate);
        else
            std::cerr << "XML error: weapon.firerate is missing" << std::endl;

        int damage = 0;
        if (elem->QueryIntAttribute("damage", &damage) == 0)
            weapon.setDamage(damage);
        else
            std::cerr << "XML error: weapon.damage is missing" << std::endl;

        int speed = 0;
        if (elem->QueryIntAttribute("speed", &speed) == 0)
            weapon.setVelociy(speed);
        else
            std::cerr << "XML error: weapon.speed is missing" << std::endl;

        elem = elem->NextSiblingElement("weapon");
    }
}


const Weapon& EntityManager::getWeapon(const std::string& id) const
{
    WeaponMap::const_iterator it = m_weapons.find(id);
    if (it == m_weapons.end())
        throw std::runtime_error("Weapon '" + id + "' not found");

    return it->second;
}


void EntityManager::loadSpaceships(const std::string& filename)
{
    // Open XML document
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename.c_str()) != 0)
    {
        std::string error = "Cannot load spaceships from " + filename + ": " + doc.GetErrorStr1();
        throw std::runtime_error(error);
    }

    tinyxml2::XMLElement* elem = doc.RootElement()->FirstChildElement("spaceship");
    while (elem != NULL)
    {
        const char* id = elem->Attribute("id");
        if (id == NULL)
            throw std::runtime_error("XML parse error: spaceship.id is missing");

        const char* animation = elem->Attribute("animation");
        if (animation == NULL)
            throw std::runtime_error("XML error: spaceship.animation is missing");

        int hp = 0;
        if (elem->QueryIntAttribute("hp", &hp) != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("XML error: spaceship.hp is missing");

        int speed = 0;
        if (elem->QueryIntAttribute("speed", &speed) != tinyxml2::XML_SUCCESS)
            throw std::runtime_error("XML error: spaceship.speed is missing");

        int points = 0;
        elem->QueryIntAttribute("points", &points);

        // Create spaceship instance
        Spaceship ship(getAnimation(animation), hp, speed);
        ship.setPoints(points);
        ship.setMovementPattern(parse_movement_pattern(elem));
        ship.setAttackPattern(parse_attack_pattern(elem));

        // Parse weapon tag
        tinyxml2::XMLElement* weapon = elem->FirstChildElement();
        if (weapon != NULL)
        {
            int wx, wy;
            const char* weapon_id = weapon->Attribute("id");
            if (weapon_id == NULL)
                throw std::runtime_error("XML error: spaceship.weapon.id is missing");

            if (weapon->QueryIntAttribute("x", &wx) != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("XML error: spaceship.weapon.x is missing");

            if (weapon->QueryIntAttribute("y", &wy) != tinyxml2::XML_SUCCESS)
                throw std::runtime_error("XML error: spaceship.weapon.y is missing");

            ship.getWeapon().init(weapon_id);
            ship.getWeapon().setPosition(wx, wy);
        }

        // Insert spaceship in Spaceship map (LEVELS_MODE: access by id)
        m_spaceships.insert(std::make_pair(id, ship));

        // Insert spaceship in sorted vector (INFINITY_MODE: random access)
        std::vector<Spaceship>::iterator it = m_sorted_ships.begin();
        while (it != m_sorted_ships.end() && it->getPoints() < ship.getPoints())
        {
            ++it;
        }
        m_sorted_ships.insert(it, ship);

        elem = elem->NextSiblingElement("spaceship");
    }
}


Spaceship* EntityManager::createSpaceship(const std::string& id) const
{
    SpaceshipMap::const_iterator it = m_spaceships.find(id);
    if (it == m_spaceships.end())
        std::cerr << "Cannot create spaceship with id '" << id << "'" << std::endl;

    return it->second.clone();
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
        m_max_droppable_points = m_timer / DROP_DELAY_STEP + 1;

        for (size_t i = m_max_droppable_index; i < m_sorted_ships.size(); ++i)
        {
            if (m_sorted_ships[i].getPoints() <= m_max_droppable_points)
            {
                m_max_droppable_index = i;
            }
            else
            {
                break;
            }
        }
        return m_sorted_ships[math::rand(0, m_max_droppable_index)].clone();
    }
}


bool EntityManager::infinityModeCallback()
{
    // Number of max entities = time / STEP + START
    const int STEP = 8;
    const int START = 1;
    if (m_entities.size() < m_timer / STEP + START)
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


bool EntityManager::levelsCallback()
{
    // Move entities from the spawn queue to the EntityManager
    Entity* next = m_levels.spawnNextEntity(m_timer);
    while (next != NULL)
    {
        next->move(next->getOrigin());
        addEntity(next);
        next = m_levels.spawnNextEntity(m_timer);
    }

    // The current level is completed when there is no remaining entities in the
    // LevelManager's spawn queue and Player is the only entity still active
    return m_levels.getSpawnQueueSize() > 0 || m_entities.size() > 1;
}


void EntityManager::respawnPlayer()
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
    particle.position.x = math::rand(0, EntityManager::getInstance().getWidth());
    particle.position.y = math::rand(0, EntityManager::getInstance().getHeight());
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

EntityManager::ParallaxLayer::ParallaxLayer():
    m_texture(NULL),
    m_blend_mode(sf::BlendAlpha)
{
}


void EntityManager::ParallaxLayer::resetScrolling()
{
    if (m_texture)
    {
        sf::Vector2u size = m_texture->getSize();

        // Set 1st image at (0, 0)
        m_vertices[0].position = sf::Vector2f(0, 0);
        m_vertices[1].position = sf::Vector2f(size.x, 0);
        m_vertices[2].position = sf::Vector2f(size.x, size.y);
        m_vertices[3].position = sf::Vector2f(0, size.y);

        // Set 2nd image at (size.x, 0)
        m_vertices[4].position = sf::Vector2f(size.x, 0);
        m_vertices[5].position = sf::Vector2f(size.x * 2, 0);
        m_vertices[6].position = sf::Vector2f(size.x * 2, size.y);
        m_vertices[7].position = sf::Vector2f(size.x, size.y);
    }
}


void EntityManager::ParallaxLayer::scroll(float delta)
{
    for (int i = 0; i < 8; ++i)
        m_vertices[i].position.x -= delta;

    if (m_vertices[1].position.x < 0)
        resetScrolling();
}


void EntityManager::ParallaxLayer::setTexture(const sf::Texture& texture)
{
    m_texture = &texture;
    sf::Vector2u size = texture.getSize();

    // Set 1st image's texture coords
    m_vertices[0].texCoords = sf::Vector2f(0, 0);
    m_vertices[1].texCoords = sf::Vector2f(size.x, 0);
    m_vertices[2].texCoords = sf::Vector2f(size.x, size.y);
    m_vertices[3].texCoords = sf::Vector2f(0, size.y);

    // Set 2nd image's texture coords
    for (int i = 0; i < 4; ++i)
        m_vertices[i + 4].texCoords = m_vertices[i].texCoords;

    resetScrolling();
}


void EntityManager::ParallaxLayer::setColor(const sf::Color& color)
{
    for (int i = 0; i < 8; ++i)
        m_vertices[i].color = color;

    // If a color is provided, layer is additive (useful for "fog" top layer)
    m_blend_mode = color != sf::Color::White ? sf::BlendAdd : sf::BlendAlpha;
}


void EntityManager::ParallaxLayer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    if (m_texture)
    {
        states.blendMode = m_blend_mode;
        states.texture = m_texture;
        target.draw(m_vertices, 8, sf::Quads, states);
    }
}

