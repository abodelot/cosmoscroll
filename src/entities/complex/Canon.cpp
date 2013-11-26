#include "Canon.hpp"
#include "Part.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

Canon::Canon()
{
	Part base;
	base.setTexture(Resources::getTexture("entities/decor-bottom.png"));

	Part top(1);
	top.setTexture(Resources::getTexture("entities/decor-canon.png"));

	addPart(base, 0, 18);
	addPart(top, (base.getWidth() - top.getWidth()) / 2, 0);

	weapon_.init("fireball", 2);
	weapon_.setOwner(this);
	weapon_.setPosition({64, 8});
}


void Canon::onInit()
{
	// Always positionned on bottom
	setY(EntityManager::getInstance().getHeight() - (18 + 128));
}


void Canon::onUpdate(float frametime)
{
	MultiPartEntity::onUpdate(frametime);
	weapon_.shoot(math::PI / 2.f);
	weapon_.onUpdate(frametime);
}
