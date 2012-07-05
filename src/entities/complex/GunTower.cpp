#include "GunTower.hpp"
#include "utils/Resources.hpp"
#include "utils/Math.hpp"
#include "entities/EntityManager.hpp"

#define BASE_OFFSET 28

GunTower::GunTower(const sf::Vector2f& position): ComplexEntity(position)
{
	SetY(EntityManager::GetInstance().GetHeight() - (BASE_OFFSET + 64));

	Part base;
	base.SetImage(Resources::GetImage("entities/guntower-base.png"));
	base.SetCollideFlag(C_IGNORE_DAMAGE);

	Part turret;
	const sf::Image& img_turret = Resources::GetImage("entities/guntower-turret.png");
	turret.SetImage(img_turret);
	turret.SetCenter(img_turret.GetWidth() / 2, img_turret.GetHeight() / 2);
	turret.SetCollideFlag(C_IGNORE_DAMAGE);

	AddPart(turret, img_turret.GetWidth() / 2, img_turret.GetHeight() / 2);
	AddPart(base, 0, BASE_OFFSET);
	target_ = NULL;

	weapon_.Init("laser-pink");
	weapon_.SetOwner(this);
	weapon_.SetOffset(img_turret.GetWidth() / 2, img_turret.GetHeight() / 2);
}


void GunTower::Update(float frametime)
{
	ComplexEntity::Update(frametime);
	weapon_.ShootAt(target_->GetCenter_());
	weapon_.Update(frametime);
	// rotate turret toward player
	GetPartAt(0)->SetRotation(180 + math::rad_to_deg(math::angle(target_->GetPosition(), GetPosition())));
}


void GunTower::SetTarget(Entity* entity)
{
	target_ = entity;
}


GunTower* GunTower::Clone() const
{
	return new GunTower(*this);
}
