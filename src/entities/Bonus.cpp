#include <cassert>
#include "Bonus.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"

#define BONUS_SPEED 100
#define SIZE        16


Bonus::Bonus(Type type, const sf::Vector2f& offset) :
	Entity(offset, 1, 0)
{
	setTexture(Resources::getTexture("entities/bonus.png"));
	setTextureRect(getTextureRect(type));
	SetCollideFlag(C_IGNORE_HITS | C_IGNORE_DAMAGE); // non-damageable, power-ups will be removed by player
	type_ = type;
}


Bonus* Bonus::Clone() const
{
	return new Bonus(*this);
}


Bonus* Bonus::MakeRandom(const sf::Vector2f& offset)
{
	Type type = (Type) math::random(0, BONUS_COUNT - 1);
	return new Bonus((Type) type, offset);
}


void Bonus::Update(float frametime)
{
	sf::Sprite::move(-BONUS_SPEED * frametime, 0);
}


const std::wstring& Bonus::GetDescription() const
{
	switch (type_)
	{
		case HEALTH:
			return _t("bonus.hp");
		case SHIELD:
			return _t("bonus.shield");
		case COOLER:
			return _t("bonus.cooler");
		case MISSILE:
			return _t("bonus.missile");
		case DOUBLE_SHOT:
			return _t("bonus.double_shot");
		case TRIPLE_SHOT:
			return _t("bonus.triple_shot");
		case SPEED:
			return _t("bonus.speed");
		case SUPER_BANANA:
			return _t("bonus.banana");
		default:
			break;
	}
	return _t("bonus.unknown");
}


Bonus::Type Bonus::GetType() const
{
	return type_;
}


sf::IntRect Bonus::getTextureRect(Type type)
{
	// l'ordre des bonus dans la feuille de sprites est le même que celui
	// de l'énumération Bonus::Type
	return sf::IntRect(type * SIZE, 0, SIZE, SIZE);
}

