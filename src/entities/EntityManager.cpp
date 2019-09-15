#include "EntityManager.hpp"
#include "Asteroid.hpp"
#include "Player.hpp"
#include "Spaceship.hpp"
#include "core/Constants.hpp"
#include "core/SoundSystem.hpp"
#include "core/LevelParser.hpp"
#include "core/Services.hpp"
#include "core/MessageSystem.hpp"
#include "core/Resources.hpp"
#include "core/Collisions.hpp"
#include "utils/SFML_Helper.hpp"

#include <stdexcept>
#include <iostream>
#include <cassert>


EntityManager& EntityManager::getInstance()
{
    static EntityManager self;
    return self;
}


EntityManager::EntityManager():
    m_view(sf::FloatRect(0, 0, APP_WIDTH, APP_HEIGHT)),
    m_timer(0),
    m_traveled(0),
    m_scrollingSpeed(90),
    m_stopScrollingAt(0),
    m_player(nullptr),
    m_particleSystem(Services::getParticleSystem()),
    m_impactEmitter(m_particleSystem),
    m_greenEmitter(m_particleSystem),
    m_starsEmitter(m_particleSystem)
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
    m_particleSystem.setTexture(&Resources::getTexture("particles/particles.png"));
    m_starsEmitter.setTextureRect(sf::IntRect(32, 9, 3, 3));
    m_starsEmitter.setLifetime(0);
    m_starsEmitter.setSpeed(450, 40);
    m_starsEmitter.setAngle(-math::PI);

    m_impactEmitter.setTextureRect(sf::IntRect(32, 8, 8, 1));
    m_impactEmitter.setScale(1.5, 0.5);
    m_impactEmitter.setSpeed(200, 50);
    m_impactEmitter.setLifetime(3);

    m_greenEmitter.setTextureRect(sf::IntRect(40, 8, 8, 8));

    m_tileset.setTileSize({16, 16});
    m_tileset.loadTexture("resources/images/levels/tileset.png");
    Services::getCollisions().registerTexture(&m_tileset.getTexture());
}


EntityManager::~EntityManager()
{
    clearSpawnQueue();
    clearEntities();
}


void EntityManager::initializeLevel()
{
    // re-init particles
    m_particleSystem.clear();
    clearSpawnQueue();

    // le vaisseau du joueur est conservé d'un niveau à l'autre
    if (m_player == nullptr || m_player->isDead()) {
        respawnPlayer();
    } else {
        // Delete all entities but player
        for (EntityList::iterator it = m_entities.begin(); it != m_entities.end();) {
            if (*it != m_player) {
                delete *it;
                it = m_entities.erase(it);
            } else {
                ++it;
            }
        }
        m_player->setPosition(0, m_size.y / 2);
        m_player->onInit();
    }

    // Reset view to initial position
    m_view.setCenter({APP_WIDTH / 2, APP_HEIGHT / 2});

    LevelParser& levelParser = Services::getLevelParser();
    m_starsEmitter.createParticles(levelParser.getStarsCount());

    // Set background music, if any
    if (levelParser.getMusicName() != nullptr) {
        std::string filename = Resources::getSearchPath() + "/music/" + levelParser.getMusicName();
        Services::getSoundSystem().openMusicFromFile(filename);
	Services::getSoundSystem().playMusic();
    } else {
        Services::getSoundSystem().stopMusic();
    }

    // Load tilemap from the image linked in the level file
    const char* path = levelParser.getTilemapPath();
    if (!path) {
        std::cerr << "Missing 'src' attribute on 'tilemap' element" << std::endl;
        return ;
    }
    m_tilemap.loadFromImage(m_tileset, path, *this);

    // Size of the manager is the size of the tilemap
    m_size = m_tilemap.getPixelSize();
    Services::getHUD().setLevelLength(m_size.x);

    m_player->setPosition(50, m_size.y / 2);
    std::cout << "EntityManager::initialize(), recenter player at " << m_player->getX() << ", " << m_player->getY() << std::endl;

    // Set scrolling speed
    m_scrollingSpeed = levelParser.getTilemapSpeed();

    // Set parallax scrolling
    m_layers.clear();
    levelParser.parseLayers(m_layers);


    m_timer = 0.f;
    m_traveled = 0;
    // Stop auto scrolling when player is near the level end
    m_stopScrollingAt = m_size.x - APP_WIDTH + 32;
}


void EntityManager::resize(int width, int height)
{
    m_size = {width, height};
}


void EntityManager::update(float frametime)
{
    const sf::FloatRect& screen = getViewZone();

    EntityList::iterator it, it2;

    // Update and collision
    for (it = m_entities.begin(); it != m_entities.end();) {
        Entity& entity = **it;
        entity.onUpdate(frametime);
        sf::FloatRect box = entity.getBoundingBox();

        if (entity.isDead()) {
            // Remove dead entities
            delete *it;
            it = m_entities.erase(it);
        } else if (sfh::disjoint(screen, box)) {
            // Remove entities outside the entity manager
            delete *it;
            it = m_entities.erase(it);
        } else {
            // Tilemap collision
            collidesWithTilemap(box, entity);

            it2 = it;
            for (++it2; it2 != m_entities.end(); ++it2) {
                // Collision dectection it1 <-> it2
                if (Services::getCollisions().pixelPerfectTest(entity, **it2)) {
                    entity.collides(**it2);
                    (**it2).collides(entity);
                }
            }
            ++it;
        }
    }

    sf::Vector2f center = m_view.getCenter();
    // Do not scroll beyond the end of level (size.x)
    center.x = std::min(center.x + m_scrollingSpeed * frametime, m_size.x - APP_WIDTH / 2.f);
    m_view.setCenter(center);

    if (m_player && m_player->getPosition().x < m_stopScrollingAt) {
        sf::Vector2f delta(m_scrollingSpeed * frametime, 0);
        m_player->move(delta);
    }

    m_particleSystem.update(frametime);

    // Parallax scrolling
    for (ParallaxLayer& layer: m_layers) {
        layer.scroll(frametime);
    }

    m_timer += frametime;
    m_traveled = m_view.getCenter().x + APP_WIDTH / 2;

    spawnEntities();
}


void EntityManager::addEntity(Entity* entity)
{
    entity->onInit();
    m_entities.push_back(entity);
}


void EntityManager::clearEntities()
{
    for (EntityList::iterator it = m_entities.begin(); it != m_entities.end(); ++it) {
        delete *it;
    }
    m_entities.clear();
}


bool EntityManager::gameOverOrCompleted() const
{
    // Player is dead
    if (m_player && m_player->isDead()) return true;

    // The current level is completed when there is no remaining entities in the
    // spawn queue and Player is the only entity still active
    if (m_spawnQueue.empty() && m_entities.size() == 1) {
        return true;
    }

    return false;
}


void EntityManager::clearSpawnQueue()
{
    // Delete remaining entities in the spawn queue
    while (!m_spawnQueue.empty()) {
        delete m_spawnQueue.top();
        m_spawnQueue.pop();
    }
}


sf::FloatRect EntityManager::getViewZone() const
{
    const sf::Vector2f& center = m_view.getCenter();
    const sf::Vector2f& size = m_view.getSize();
    return sf::FloatRect(center.x - size.x / 2, center.y - size.y / 2, size.x, size.y);
}


void EntityManager::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Draw layers for parallax scrolling
    for (const ParallaxLayer& layer: m_layers) {
        target.draw(layer, states);
    }

    target.setView(m_view);

    // Draw particles
    target.draw(m_particleSystem, states);

    // Draw tilemap
    target.draw(m_tilemap, states);

    // Draw entities
    for (const Entity* entity: m_entities) {
        target.draw(*entity, states);
    }

    target.draw(m_collision);
    for (int i =0; i < 8; ++i) {
        target.draw(m_allCheckTiles[i]);
        target.draw(m_allPoints[i]);
    }
    target.draw(m_collisionPoint);
}


Player* EntityManager::getPlayer() const
{
    assert(m_player != nullptr);
    return m_player;
}


void EntityManager::scheduleForSpawning(Entity* entity)
{
    m_spawnQueue.push(entity);
}


void EntityManager::spawnEntities()
{
    while (!m_spawnQueue.empty()) {
        Entity* next = m_spawnQueue.top();
        if (next->getX() > m_traveled) {
            break;
        }

        // Pop entity from the spawn queue if the traveled distance has reached its spawn position
        // Popped entities are moved to the active entity list
        m_spawnQueue.pop();
        addEntity(next);
    }
}


void EntityManager::respawnPlayer()
{
    std::cout << "Respawn Player" << std::endl;
    clearEntities();
    m_player = new Player();
    m_player->setPosition(50, m_size.y / 2);
    addEntity(m_player);
}


void EntityManager::toggleFastScrolling()
{
    float scrollingSpeed = Services::getInstance().getLevelParser().getTilemapSpeed();
    m_scrollingSpeed = m_scrollingSpeed == scrollingSpeed ? scrollingSpeed * 5 : scrollingSpeed;
}


void EntityManager::createImpactParticles(const sf::Vector2f& pos, size_t count)
{
    m_impactEmitter.setPosition(pos);
    m_impactEmitter.createParticles(count);
}


void EntityManager::createGreenParticles(const sf::Vector2f& pos, size_t count)
{
    m_greenEmitter.setPosition(pos);
    m_greenEmitter.createParticles(count);
}


bool EntityManager::collidesWithTilemap(const sf::FloatRect& rect, Entity& entity) const
{
    sf::Sprite sprite;
    // 8 points detection
    const sf::Vector2f points[] = {
        {rect.left,                  rect.top}, // top-left
        {rect.left + rect.width / 2, rect.top}, // top-middle
        {rect.left + rect.width,     rect.top}, // top-right
        {rect.left,                  rect.top + rect.height / 2},  // middle left
        {rect.left + rect.width,     rect.top + rect.height / 2}, // middle right
        {rect.left ,                 rect.top + rect.height}, // bottom left
        {rect.left + rect.width / 2, rect.top + rect.height}, // bottom middle
        {rect.left + rect.width,     rect.top + rect.height} // bottom right
    };

#ifdef DEBUG
    if (m_player == &entity) {
        for (int i = 0; i < 8; ++i) {
            m_allPoints[i].setPosition(points[i].x - 1, points[i].y - 1);
            m_allPoints[i].setFillColor(sf::Color::Green);
            m_allPoints[i].setSize({2.f, 2.f});

            sf::Vector2i coords = m_tilemap.convertPixelsToCoords(points[i]);
            sprite = m_tilemap.getSpriteAt(coords);
            m_allCheckTiles[i].setPosition(sprite.getPosition());
            m_allCheckTiles[i].setFillColor(sf::Color(0, 255, 0, 128));
            m_allCheckTiles[i].setSize({sprite.getLocalBounds().width, sprite.getLocalBounds().height});
        }
    }
#endif

    for (int i = 0; i < 8; ++i) {
        sf::Vector2i coords = m_tilemap.convertPixelsToCoords(points[i]);
        if (m_tilemap.hasTile(coords)) {
            sprite = m_tilemap.getSpriteAt(coords);
#ifdef DEBUG
            if (m_player == &entity) {
                m_allCheckTiles[i].setFillColor(sf::Color::Red);
            }
#endif
            if (Services::getCollisions().pixelPerfectTest(entity, sprite)) {
                entity.onTileCollision();
                return true;
            }
        }
    }
    return false;
}

// StarsEmitter ----------------------------------------------------------------

void EntityManager::StarsEmitter::onParticleCreated(Particle& particle) const
{
    const sf::FloatRect& zone = EntityManager::getInstance().getViewZone();
    particle.position.x = math::rand(zone.left, zone.left + zone.width);
    particle.position.y = math::rand(zone.top, zone.top + zone.height);
}


void EntityManager::StarsEmitter::onParticleUpdated(Particle& particle, float) const
{
    const sf::FloatRect& zone = EntityManager::getInstance().getViewZone();
    if (particle.position.x < zone.left) {
        particle.position.x = zone.left + zone.width;
    }
}

// PosComparator

bool EntityManager::PosComparator::operator()(const Entity* a, const Entity* b) const
{
    return a->getX() > b->getX();
}
