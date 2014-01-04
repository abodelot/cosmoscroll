#include "Entity.hpp"
#include "core/Collisions.hpp"

// BoundingBox -----------------------------------------------------------------

Entity::BoundingBox::BoundingBox():
	left(0.f),
	top(0.f),
	right(0.f),
	bottom(0.f)
{
}


Entity::BoundingBox::BoundingBox(float l, float t, float r, float b):
	left(l),
	top(t),
	right(r),
	bottom(b)
{
}

// Entity ----------------------------------------------------------------------

Entity::Entity():
	m_dead(false),
	m_team(NEUTRAL)
{

}


bool Entity::isDead() const
{
	return m_dead;
}


void Entity::kill()
{
	m_dead = true;
	onDestroy();
}


Entity::Team Entity::getTeam() const
{
	return m_team;
}


void Entity::setTeam(Team team)
{
	m_team = team;
}


Entity::BoundingBox Entity::getBoundingBox() const
{
	sf::Vector2f pos = getPosition() - getOrigin();
	return BoundingBox(pos.x, pos.y, pos.x + getTextureRect().width, pos.y + getTextureRect().height);
}


sf::Vector2f Entity::getCenter() const
{
	sf::Vector2f pos = getPosition() - getOrigin();
	pos.x += getTextureRect().width / 2;
	pos.y += getTextureRect().height / 2;
	return pos;
}


void Entity::setTexture(const sf::Texture& texture)
{
	sf::Sprite::setTexture(texture);
	Collisions::registerTexture(&texture);
}
