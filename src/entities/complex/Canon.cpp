#include "Canon.hpp"
#include "Part.hpp"
#include "entities/EntityManager.hpp"
#include "utils/Resources.hpp"
#include "utils/Math.hpp"

Canon::Canon(const sf::Vector2f& position): ComplexEntity(position)
{
	SetY(EntityManager::GetInstance().GetHeight() - (18 + 128));

	Part base;
	base.SetImage(Resources::GetImage("entities/decor-bottom.png"));
	base.SetCollideFlag(C_IGNORE_DAMAGE);

	Part top(1);
	top.SetImage(Resources::GetImage("entities/decor-canon.png"));
	top.SetCollideFlag(C_IGNORE_DAMAGE);

	AddPart(base, 0, 18);
	AddPart(top, (base.GetSize().x - top.GetSize().x) / 2, 0);

	weapon_.Init("decor-canon");
	weapon_.SetOwner(this);
	weapon_.SetOffset(64, 8);
}


void Canon::Update(float frametime)
{
	ComplexEntity::Update(frametime);
	weapon_.Shoot(PI / 2.f);
	weapon_.Update(frametime);
}

Canon* Canon::Clone() const
{
	return new Canon(*this);
}
