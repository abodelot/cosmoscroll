#include "FlyingSaucerBoss.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"
#include "entities/Spaceship.hpp"
#include "core/Factory.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

#include <utils/SFML_Helper.hpp>
#include <iostream>

static constexpr float CIRCLE_RADIUS = 60.f;
#define CIRCLE_ROTATION_SPEED (math::PI * 0.8)


FlyingSaucerBoss::FlyingSaucerBoss():
    m_target(nullptr),
    m_timer(0),
    m_angle(0)
{
    setHP(400);
    setTexture(Resources::getTexture("entities/flying-saucer-boss.png"));

    m_leftTube.init("laser-pink");
    m_leftTube.setOwner(this);
    m_leftTube.setPosition(34, 122);
    m_rightTube.init("laser-pink");
    m_rightTube.setOwner(this);
    m_rightTube.setPosition(157, 122);
    setTeam(Entity::BAD);
}


void FlyingSaucerBoss::setSpawnPosition(const sf::Vector2f& position)
{
    m_spawnPosition = position;
    Entity::setSpawnPosition(position);
}


void FlyingSaucerBoss::onUpdate(float frametime)
{
    // Attack
    sf::Vector2f target_pos = m_target->getCenter();

    m_leftTube.shoot(target_pos);
    m_rightTube.shoot(target_pos);

    sf::Vector2f pos(m_spawnPosition);
    m_angle += CIRCLE_ROTATION_SPEED * frametime;
    pos.x += CIRCLE_RADIUS * std::cos(m_angle);
    pos.y -= CIRCLE_RADIUS * std::sin(m_angle);
    setPosition(pos);
    updateDamageFlash(frametime);

    m_timer += frametime;
    if (m_timer > 5.f) {
        m_timer = 0.f;
        // FIXME: fetch color code for flying saucer mob
        // Spaceship* mob = g_factory.createSpaceship(0xff0000);
        // mob->setPosition(getCenter());
        // EntityManager::getInstance().addEntity(mob);
    }
}


void FlyingSaucerBoss::takeDamage(int damage)
{
    Damageable::takeDamage(damage);
}


void FlyingSaucerBoss::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();
}
