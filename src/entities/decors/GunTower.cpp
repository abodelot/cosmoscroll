#include "GunTower.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"

#define BASE_ID  0
#define CANON_ID 1

GunTower::GunTower():
    m_target(nullptr)
{
    Part base(BASE_ID, 16);
    base.setTexture(Resources::getTexture("entities/guntower-base.png"));

    Part turret(CANON_ID, 16);

    const sf::Texture& imgTurret = Resources::getTexture("entities/guntower-turret.png");
    turret.setTexture(imgTurret);
    float halfTurret = imgTurret.getSize().x / 2.f;
    turret.setOrigin(halfTurret, halfTurret);
    addPart(turret, halfTurret, halfTurret);
    addPart(base, 0, halfTurret + 4);

    m_weapon.init("egg");
    m_weapon.setOwner(this);
    m_weapon.setPosition(halfTurret, halfTurret);
}


void GunTower::onUpdate(float frametime)
{
    updateParts(frametime);

    // Rotate turret toward player
    Part& turret = getPartAt(0);
    if (!turret.isDead()) {
        turret.setRotation(180 - math::to_degrees(math::angle(getPosition(), m_target->getPosition())));
        m_weapon.shoot(m_target->getCenter());
    }
}


void GunTower::onInit()
{
    m_target = EntityManager::getInstance().getPlayer();
}


void GunTower::onPartDestroyed(const Part& part)
{
    if (part.getID() == BASE_ID) {
        kill();
    }
}
