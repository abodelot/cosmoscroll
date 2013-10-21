#include "MultiPartEntity.hpp"
#include "utils/Math.hpp"
#include "core/Collisions.hpp"
#include "entities/EntityManager.hpp"


MultiPartEntity::MultiPartEntity()
{
	setTeam(Entity::NEUTRAL);
}


void MultiPartEntity::collides(Entity& entity)
{
	for (PartVector::iterator part = m_parts.begin(); part != m_parts.end(); ++part)
	{
		if (!part->isDead())
		{
			// HACK: the part's position is relative to the MultiPartEntity's position,
			// so we need to move the part to its absolute position to test the
			// collision
			sf::Vector2f relative_pos = part->getPosition();
			part->move(getPosition());
			if (Collisions::pixelPerfectTest(entity, *part))
			{
				// Restore original Part's position
				part->setPosition(relative_pos);
				entity.collides(*part);
				break;
			}
			part->setPosition(relative_pos);
		}
	}
}


void MultiPartEntity::onUpdate(float frametime)
{
	sf::Sprite::move(-EntityManager::FOREGROUND_SPEED * frametime, 0.f);
	for (size_t i = 0; i < m_parts.size(); ++i)
	{
		m_parts[i].onUpdate(frametime);
	}
}


float MultiPartEntity::getSpeedX() const
{
	return -EntityManager::FOREGROUND_SPEED;
}


void MultiPartEntity::addPart(Part& part, float x, float y)
{
	part.setPosition(x, y);
	part.setParent(this);
	m_parts.push_back(part);

	float width = x + part.getWidth();
	float height = y + part.getHeight();
	float max_width = math::max(width, getWidth());
	float max_height = math::max(height, getHeight());
	setTextureRect(sf::IntRect(0, 0, max_width, max_height));
}



Part& MultiPartEntity::getPartAt(size_t index)
{
	return m_parts.at(index);
}


Part* MultiPartEntity::getPartByID(int id)
{
	for (size_t i = 0; i < m_parts.size(); ++i)
	{
		if (m_parts[i].getID() == id)
		{
			return &m_parts[i];
		}
	}
	return NULL;
}


int MultiPartEntity::destroyPartByID(int id)
{
	int nb_destroyed = 0;
	for (size_t i = 0; i < m_parts.size(); ++i)
	{
		Part& part = m_parts[i];
		if (part.getID() == id && part.getHP() > 0)
		{
			part.kill();
			++nb_destroyed;
		}
	}
	return nb_destroyed;
}


void MultiPartEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (size_t i = 0; i < m_parts.size(); ++i)
	{
		const Part& p = m_parts[i];
		if (p.getHP() > 0)
			target.draw(m_parts[i], states);
	}
}
