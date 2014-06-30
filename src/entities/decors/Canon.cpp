#include "Canon.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"


Canon::Canon()
{
	Part base;
	base.setTexture(Resources::getTexture("entities/decor-bottom.png"));
	base.setDestructible(false);
	addPart(base, 0, 18);

	Part top(1);
	top.setTexture(Resources::getTexture("entities/decor-canon.png"));
	top.setDestructible(false);
	addPart(top, (base.getWidth() - top.getWidth()) / 2, 0);

	m_weapon.init("fireball");
	m_weapon.setOwner(this);
	m_weapon.setPosition(64, 8);
}


void Canon::onInit()
{
	// Always positionned on bottom
	setY(EntityManager::getInstance().getHeight() - (18 + 128));
}


void Canon::onUpdate(float frametime)
{
	move(-EntityManager::FOREGROUND_SPEED * frametime, 0.f);
	updateParts(frametime);

	m_weapon.shoot(math::PI / 2.f);
}
