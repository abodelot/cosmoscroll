#include "GunTower.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"
#include "entities/EntityManager.hpp"

#define BASE_OFFSET 28

GunTower::GunTower(const sf::Vector2f& position): ComplexEntity(position)
{
	setY(EntityManager::GetInstance().GetHeight() - (BASE_OFFSET + 64));

	Part base;
	base.setTexture(Resources::getTexture("entities/guntower-base.png"));
	base.SetCollideFlag(C_IGNORE_DAMAGE);

	Part turret;
	const sf::Texture& img_turret = Resources::getTexture("entities/guntower-turret.png");
	turret.setTexture(img_turret);
	turret.setOrigin(img_turret.getSize().x / 2, img_turret.getSize().y / 2);
	turret.SetCollideFlag(C_IGNORE_DAMAGE);

	AddPart(turret, img_turret.getSize().x / 2, img_turret.getSize().y / 2);
	AddPart(base, 0, BASE_OFFSET);
	target_ = NULL;

	weapon_.Init("laser-pink");
	weapon_.SetOwner(this);
	weapon_.SetOffset(img_turret.getSize().x / 2, img_turret.getSize().y / 2);
}


void GunTower::Update(float frametime)
{
	ComplexEntity::Update(frametime);
	weapon_.ShootAt(target_->getCenter());
	weapon_.Update(frametime);
	// rotate turret toward player
	GetPartAt(0)->setRotation(180 - math::to_deg(math::angle(target_->getPosition(), getPosition())));
}


void GunTower::SetTarget(Entity* entity)
{
	target_ = entity;
}


GunTower* GunTower::Clone() const
{
	return new GunTower(*this);
}
