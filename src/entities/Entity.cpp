#include "Entity.hpp"
#include "core/Collisions.hpp"

#define FLASH_DELAY 0.5f
#include <cstdio>


Entity::Entity(const sf::Vector2f& position, int hp, int collide_damage)
{
	setPosition(position);
	team_ = NEUTRAL;
	hp_ = hp;
	collide_damage_ = collide_damage;
	collide_flag_ = 0;
	points_ = 0;
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

void Entity::SetTarget(Entity*)
{
}


void Entity::TakeDamage(int damage)
{
	hp_ -= damage;
	if (hp_ <= 0)
	{
		OnDestroy();
	}
	else if (flash_timer_ <= 0)
	{
		flash_timer_ = FLASH_DELAY;
	}
}


void Entity::OnCollide(Entity& entity)
{
	if (team_ != entity.team_ && !(entity.collide_flag_ & C_IGNORE_DAMAGE))
	{
		entity.TakeDamage(collide_damage_);
	}
}


void Entity::Kill()
{
	hp_ = 0;
}


int Entity::GetHP() const
{
	return hp_;
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


Entity::Team Entity::GetTeam() const
{
	return team_;
}


int Entity::GetCollideDamage() const
{
	return collide_damage_;
}


void Entity::SetTeam(Team team)
{
	team_ = team;
}


int Entity::UpdateHP(int diff)
{
	hp_ += diff;
	return hp_;
}


void Entity::SetHP(int hp)
{
	hp_ = hp;
}


void Entity::SetPoints(int points)
{
	points_ = points;
}


int Entity::GetPoints() const
{
	return points_;
}


int Entity::ConsumePoints()
{
	int r = points_;
	points_ = 0;
	return r;
}


void Entity::SetCollideDamage(int damage)
{
	collide_damage_ = damage;
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


void Entity::SetCollideFlag(int collide_flag)
{
	collide_flag_ = collide_flag;
}


int Entity::GetCollideFlag() const
{
	return collide_flag_;
}
