#include "GunTower.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"
#include "entities/EntityManager.hpp"
#include "entities/Player.hpp"

#define BASE_OFFSET 28

GunTower::GunTower(const sf::Vector2f& position): ComplexEntity(position)
{
	setY(EntityManager::getInstance().getHeight() - (BASE_OFFSET + 64));

	Part base;
	base.setTexture(Resources::getTexture("entities/guntower-base.png"));

	Part turret;
	const sf::Texture& img_turret = Resources::getTexture("entities/guntower-turret.png");
	turret.setTexture(img_turret);
	turret.setOrigin(img_turret.getSize().x / 2, img_turret.getSize().y / 2);

	AddPart(turret, img_turret.getSize().x / 2, img_turret.getSize().y / 2);
	AddPart(base, 0, BASE_OFFSET);
	target_ = NULL;

	weapon_.init("laser-pink");
	weapon_.setOwner(this);
	weapon_.setPosition(img_turret.getSize().x / 2, img_turret.getSize().y / 2);
}


void GunTower::onUpdate(float frametime)
{
	ComplexEntity::onUpdate(frametime);
	weapon_.shoot(target_->getCenter());
	weapon_.onUpdate(frametime);
	// rotate turret toward player
	GetPartAt(0)->setRotation(180 - math::to_deg(math::angle(target_->getPosition(), getPosition())));
}


void GunTower::onInit()
{
	target_ = EntityManager::getInstance().GetPlayerShip();
}


GunTower* GunTower::clone() const
{
	return new GunTower(*this);
}
