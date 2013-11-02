#include "Part.hpp"
#include "MultiPartEntity.hpp"
#include "entities/Explosion.hpp"
#include "entities/EntityManager.hpp"


Part::Part(int id, int hp):
	m_id(id),
	m_destructible(false),
	m_parent(NULL)
{
	setHP(hp);
}


Entity* Part::clone() const
{
	return new Part(*this);
}


void Part::onUpdate(float frametime)
{
	updateDamageFlash(frametime);
}


void Part::takeDamage(int damage)
{
	if (m_destructible)
	{
		Damageable::takeDamage(damage);
	}
}


int Part::getID() const
{
	return m_id;
}


void Part::setDestructible(bool destructible)
{
	m_destructible = destructible;
}


void Part::setParent(MultiPartEntity* parent)
{
	m_parent = parent;
}


void Part::onDestroy()
{
	m_parent->onPartDestroyed(*this);

	Explosion* explosion = new Explosion;
	sf::Vector2f pos = m_parent->getPosition() + getCenter();
	explosion->setPosition(pos);
	EntityManager::getInstance().addEntity(explosion);
}
