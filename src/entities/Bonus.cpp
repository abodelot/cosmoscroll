#include <cassert>
#include <SFML/System/Randomizer.hpp>

#include "Bonus.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/I18n.hpp"

#define BONUS_SPEED 100
#define SIZE        16


Bonus::Bonus(Type type, const sf::Vector2f& offset) :
	Entity(offset, 1, 0)
{
	SetImage(GET_IMG("entities/bonus"));
	SetSubRect(GetSubRect(type));
	type_ = type;
}


Bonus* Bonus::Clone() const
{
	return new Bonus(*this);
}


Bonus* Bonus::MakeRandom(const sf::Vector2f& offset)
{
	Type type = (Type) sf::Randomizer::Random(0, BONUS_COUNT - 1);
	return new Bonus((Type) type, offset);
}


void Bonus::TakeDamage(int)
{
	// un bonus ne peut être détruit
}


void Bonus::Update(float frametime)
{
	sf::Sprite::Move(-BONUS_SPEED * frametime, 0);
	KillIfOut();
}


const wchar_t* Bonus::GetDescription() const
{
	switch (type_)
	{
		case HEALTH:
			return I18n::t("bonus.hp").c_str();
		case SHIELD:
			return I18n::t("bonus.shield").c_str();
		case COOLER:
			return I18n::t("bonus.cooler").c_str();
		case MISSILE:
			return I18n::t("bonus.missile").c_str();
		case DOUBLE_SHOT:
			return I18n::t("bonus.double_shot").c_str();
		case TRIPLE_SHOT:
			return I18n::t("bonus.triple_shot").c_str();
		case SPEED:
			return I18n::t("bonus.speed").c_str();
		case SUPER_BANANA:
			return I18n::t("bonus.banana").c_str();
		default:
			break;
	}
	return L"<unknow bonus>";
}


Bonus::Type Bonus::GetType() const
{
	return type_;
}


sf::IntRect Bonus::GetSubRect(Type type)
{
	// l'ordre des bonus dans la feuille de sprites est le même que celui
	// de l'énumération Bonus::Type
	return sf::IntRect(type * SIZE, 0, (type + 1) * SIZE, SIZE);
}

