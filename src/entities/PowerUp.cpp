#include "PowerUp.hpp"
#include "EntityManager.hpp"
#include "core/Resources.hpp"
#include "core/Services.hpp"
#include "utils/Math.hpp"


PowerUp::PowerUp(Type type):
    m_type(type)
{
    setTexture(Resources::getTexture("entities/power-ups.png"));
    setTextureRect(getTextureRect(type));
}


void PowerUp::collides(Entity& entity)
{
    entity.onCollision(*this);
}


void PowerUp::dropRandom(const sf::Vector2f& position)
{
    PowerUp* powerup = new PowerUp((Type) math::rand(0, PowerUp::_COUNT - 1));
    powerup->setPosition(position);
    EntityManager::getInstance().addEntity(powerup);
}


sf::String PowerUp::getDescription() const
{
    switch (m_type) {
        case REPAIR:      return _t("powerup.repair");
        case FULL_REPAIR: return _t("powerup.full_repair");
        case SHIELD:      return _t("powerup.shield");
        case FULL_SHIELD: return _t("powerup.full_shield");
        case ICECUBE:     return _t("powerup.icecube");
        case MISSILE:     return _t("powerup.missile");
        case DOUBLE_SHOT: return _t("powerup.double_shot");
        case TRIPLE_SHOT: return _t("powerup.triple_shot");
        case SPEED:       return _t("powerup.speed");
        default:          return "";
    }
}


PowerUp::Type PowerUp::getType() const
{
    return m_type;
}


sf::IntRect PowerUp::getTextureRect(Type type)
{
    // PowerUp::Type enumeration has the same order than the spritesheet (16x16 resolution)
    return sf::IntRect(type * 16, 0, 16, 16);
}
