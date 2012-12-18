#include "Canon.hpp"
#include "Part.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "utils/Math.hpp"

Canon::Canon(const sf::Vector2f& position): ComplexEntity(position)
{
	setY(EntityManager::GetInstance().GetHeight() - (18 + 128));

	Part base;
	base.setTexture(Resources::getTexture("entities/decor-bottom.png"));
	base.SetCollideFlag(C_IGNORE_DAMAGE);

	Part top(1);
	top.setTexture(Resources::getTexture("entities/decor-canon.png"));
	top.SetCollideFlag(C_IGNORE_DAMAGE);

	AddPart(base, 0, 18);
	AddPart(top, (base.getWidth() - top.getWidth()) / 2, 0);

	weapon_.init("decor-canon");
	weapon_.setOwner(this);
	weapon_.setPosition(64, 8);
}


void Canon::Update(float frametime)
{
	ComplexEntity::Update(frametime);
	weapon_.shoot(math::PI / 2.f);
	weapon_.onUpdate(frametime);
}

Canon* Canon::Clone() const
{
	return new Canon(*this);
}
