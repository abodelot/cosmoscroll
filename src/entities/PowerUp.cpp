#include "PowerUp.hpp"
#include "Player.hpp"
#include "EntityManager.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"

#define POWERUP_SPEED 100
#define SIZE          16


PowerUp::PowerUp(Type type, const sf::Vector2f& position) :
	Entity(position, 1),
	m_type(type)
{
	setTexture(Resources::getTexture("entities/bonus.png"));
	setTextureRect(getTextureRect(type));
}


PowerUp* PowerUp::clone() const
{
	return new PowerUp(*this);
}


void PowerUp::onUpdate(float frametime)
{
	sf::Sprite::move(-POWERUP_SPEED * frametime, 0);
}


void PowerUp::onCollision(const Entity& entity)
{
	if (entity.toPlayer())
	{
		kill();
	}
}


const PowerUp* PowerUp::toPowerUp() const
{
	return this;
}


void PowerUp::dropRandom(const sf::Vector2f& position)
{
	Type type = (Type) math::random(0, POWERUP_COUNT - 1);
	EntityManager::getInstance().addEntity(new PowerUp((Type) type, position));
}


const std::wstring& PowerUp::getDescription() const
{
	switch (m_type)
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


PowerUp::Type PowerUp::getType() const
{
	return m_type;
}


sf::IntRect PowerUp::getTextureRect(Type type)
{
	// l'ordre des bonus dans la feuille de sprites est le même que celui
	// de l'énumération Bonus::Type
	return sf::IntRect(type * SIZE, 0, SIZE, SIZE);
}

