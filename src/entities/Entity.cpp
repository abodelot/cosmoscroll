#include "Entity.hpp"
#include "../core/Window.hpp"


Entity::Entity(const sf::Vector2f& position, int hp, int collide_damage)
{
	SetPosition(position);

	team_ = NEUTRAL;
	hp_ = hp;
	flipped_ = false;
	collide_damage_ = collide_damage;
}


Entity::~Entity()
{
}


void Entity::SetTarget(Entity*)
{
}


void Entity::TakeDamage(int damage)
{
	hp_ -= damage;
}


void Entity::OnCollide(Entity& entity)
{
	if (team_ != entity.team_)
	{
		entity.TakeDamage(collide_damage_);
	}
}


void Entity::Kill()
{
	hp_ = 0;
}


void Entity::KillIfOut()
{
	if (outside_universe(GetCollideRect()))
	{
		hp_ = 0;
#ifdef DEBUG_ENTITY
		sf::Vector2f pos = GetPosition();
		printf("(entity is out : x %f, y %f)\n", pos.x, pos.y);
#endif
	}
}


sf::FloatRect Entity::GetCollideRect() const
{
	sf::FloatRect rect;
	rect.Left = GetPosition().x;
	rect.Top = GetPosition().y;
	rect.Right = rect.Left + GetSize().x;
	rect.Bottom = rect.Top + GetSize().y;
	return rect;
}


void Entity::GetCollideRect(sf::FloatRect& rect) const
{
	rect.Left = GetPosition().x;
	rect.Top = GetPosition().y;
	rect.Right = rect.Left + GetSize().x;
	rect.Bottom = rect.Top + GetSize().y;
}


Entity::Team Entity::GetTeam() const
{
	return team_;
}


void Entity::SetTeam(Team team)
{
	team_ = team;
}



