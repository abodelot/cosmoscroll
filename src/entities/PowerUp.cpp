#include "PowerUp.hpp"
#include "EntityManager.hpp"
#include "core/Resources.hpp"
#include "utils/I18n.hpp"
#include "utils/Math.hpp"
#include "utils/SFML_Helper.hpp"

#define POWERUP_SPEED 100
#define SIZE          16


PowerUp::PowerUp(Type type):
	m_type(type)
{
	setTexture(Resources::getTexture("entities/bonus.png"));
	setTextureRect(getTextureRect(type));
}


void PowerUp::collides(Entity& entity)
{
	entity.onCollision(*this);
}


void PowerUp::dropRandom(const sf::Vector2f& position)
{
	PowerUp* powerup = new PowerUp((Type) xsf::random(0, POWERUP_COUNT - 1));
	powerup->setPosition(position);
	EntityManager::getInstance().addEntity(powerup);
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
	// Bonus::Type enumeration has the same order than the spritesheet
	return sf::IntRect(type * SIZE, 0, SIZE, SIZE);
}

// callbacks -------------------------------------------------------------------

void PowerUp::onUpdate(float frametime)
{
	move(-POWERUP_SPEED * frametime, 0);
}

