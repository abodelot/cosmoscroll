#include "Entity.hpp"
#include "core/Collisions.hpp"
#include "complex/ComplexEntity.hpp"
#include "complex/Part.hpp"

#define FLASH_DELAY 0.5f


Entity::Entity(const sf::Vector2f& position, int hp):
	m_hp(hp),
	m_points(0),
	m_team(NEUTRAL)
{
	setPosition(position);
	flash_timer_ = 0.f;
}


Entity::~Entity()
{
}


sf::Vector2f Entity::getCenter() const
{
	sf::Vector2f pos = getPosition();
	pos.x += getTextureRect().width / 2;
	pos.y += getTextureRect().height / 2;
	return pos;
}


void Entity::setTexture(const sf::Texture& texture)
{
	sf::Sprite::setTexture(texture);
	Collisions::registerTexture(&texture);
}


void Entity::takeDamage(int damage)
{
	if (isDead())
		return;

	m_hp -= damage;
	if (m_hp <= 0)
	{
		onDestroy();
	}
	else if (flash_timer_ <= 0)
	{
		flash_timer_ = FLASH_DELAY;
	}
}


void Entity::kill()
{
	m_hp = 0;
}


int Entity::getHP() const
{
	return m_hp;
}


void Entity::onCollision(const Entity& entity)
{
	// This is the default collision behavior, suitable for most entities
	// See overriden methods for specific behavior

	if (entity.toPowerUp() != NULL) // ignore power ups
		return;

	if (entity.toComplexEntity() != NULL)
		return;

	else if (m_team != entity.m_team)
	{
		takeDamage(entity.m_hp);
	}
}


sf::FloatRect Entity::GetCollideRect() const
{
	const sf::Vector2f& pos = getPosition();
	const sf::Vector2f& origin = getOrigin();

	sf::FloatRect rect;
	rect.left = pos.x - origin.x;
	rect.top = pos.y - origin.y;
	rect.width = getTextureRect().width;
	rect.height = getTextureRect().height;
	return rect;
}


Entity::Team Entity::getTeam() const
{
	return m_team;
}


void Entity::setTeam(Team team)
{
	m_team = team;
}


int Entity::UpdateHP(int diff)
{
	m_hp += diff;
	return m_hp;
}


void Entity::setHP(int hp)
{
	m_hp = hp;
}


void Entity::setPoints(int points)
{
	m_points = points;
}


int Entity::getPoints() const
{
	return m_points;
}


void Entity::UpdateFlash(float frametime)
{
	if (flash_timer_ >= 0)
	{
		flash_timer_ -= frametime;
		int value = 255 - (255 * flash_timer_ / FLASH_DELAY);
		setColor(sf::Color(255, value, value));
		if (flash_timer_ <= 0)
		{
			setColor(sf::Color::White);
		}
	}
}
