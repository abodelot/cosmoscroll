#include "Part.hpp"
#include "core/ParticleSystem.hpp"
#include "ComplexEntity.hpp"


Part::Part(int id):
	Entity(sf::Vector2f(0, 0), 1),
	m_id(id),
	m_destructible(false),
	m_parent(NULL)
{
	setTeam(Entity::NEUTRAL);
}


void Part::onUpdate(float frametime)
{
	Entity::UpdateFlash(frametime);
}


void Part::onCollision(const Entity& entity)
{
	if (m_destructible)
	{
		Entity::onCollision(entity);
	}
}


Entity* Part::clone() const
{
	return new Part(*this);
}


int Part::GetID() const
{
	return m_id;
}


void Part::setDestructible(bool destructible)
{
	m_destructible = destructible;
}


void Part::setParent(ComplexEntity* parent)
{
	m_parent = parent;
}


void Part::onDestroy()
{
	m_parent->onPartDestroyed(*this);
	sf::Vector2f pos = m_parent->getPosition();
	pos.x += getCenter().x;
	pos.y += getCenter().y;
	ParticleSystem::GetInstance().ExplosionSfx(pos);
}
