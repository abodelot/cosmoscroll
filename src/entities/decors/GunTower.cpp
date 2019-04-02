#include "GunTower.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"

#define BASE_OFFSET 28

#define BASE_ID  0
#define CANON_ID 1

GunTower::GunTower():
    m_target(NULL)
{
    Part base(BASE_ID);
    base.setDestructible(false);
    base.setTexture(Resources::getTexture("entities/guntower-base.png"));

    Part turret(CANON_ID, 16);

    const sf::Texture& img_turret = Resources::getTexture("entities/guntower-turret.png");
    turret.setTexture(img_turret);
    turret.setOrigin(img_turret.getSize().x / 2, img_turret.getSize().y / 2);
    addPart(turret, img_turret.getSize().x / 2, img_turret.getSize().y / 2);
    addPart(base, 0, BASE_OFFSET);

    m_weapon.init("laser-pink");
    m_weapon.setOwner(this);
    m_weapon.setPosition(img_turret.getSize().x / 2.f, img_turret.getSize().y / 2.f);
}


void GunTower::onUpdate(float frametime)
{
    move(-EntityManager::FOREGROUND_SPEED * frametime, 0.f);
    updateParts(frametime);

    // Rotate turret toward player
    Part& turret = getPartAt(0);
    if (!turret.isDead())
    {
        turret.setRotation(180 - math::to_degrees(math::angle(getPosition(), m_target->getPosition())));
        m_weapon.shoot(m_target->getCenter());
    }
}


void GunTower::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();

    // Always positionned at bottom
    setY(EntityManager::getInstance().getHeight() - (BASE_OFFSET + 64));
}
