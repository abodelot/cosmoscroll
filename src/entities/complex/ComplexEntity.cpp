#include "ComplexEntity.hpp"
#include "utils/Math.hpp"
#include "core/ParticleSystem.hpp"
#include "core/Collisions.hpp"


ComplexEntity::ComplexEntity(const sf::Vector2f& pos):
	Entity(pos, 1)
{
	SetTeam(Entity::NEUTRAL);
	SetCollideFlag(C_IGNORE_HITS | C_IGNORE_DAMAGE);
	SetCollideDamage(0);
}


void ComplexEntity::Update(float frametime)
{
	sf::Sprite::move(-EntityManager::FOREGROUND_SPEED * frametime, 0.f);
	for (size_t i = 0; i < parts_.size(); ++i)
	{
		parts_[i].Update(frametime);
	}
}


void ComplexEntity::OnCollide(Entity& entity)
{
	for (PartVector::iterator it = parts_.begin(); it != parts_.end(); ++it)
	{
		Part& part = *it;
		if (!part.IsDead())
		{
			// HACK: collide rect is relative to ComplexEntity position
			// thus, compute part actual position, and then set back relative position
			sf::Vector2f pos = part.getPosition();
			part.move(getPosition());
			if (Collisions::pixelPerfectTest(entity, part))
			{
				entity.OnCollide(part);
				part.OnCollide(entity);
				part.setPosition(pos); // restore original position
				if (part.IsDead())
				{
					OnPartDestroyed(part);
					sf::Vector2f pos;
					pos.x = getPosition().x + part.getCenter().x;
					pos.y = getPosition().y + part.getCenter().y;
					ParticleSystem::GetInstance().ExplosionSfx(pos);
				}
				break;
			}
			part.setPosition(pos);
		}
	}
}


float ComplexEntity::GetSpeedX() const
{
	return -EntityManager::FOREGROUND_SPEED;
}


void ComplexEntity::AddPart(Part& part, float x, float y)
{
	part.setPosition(x, y);
	//part.SetTeam(GetTeam());
	parts_.push_back(part);

	float width = x + part.getWidth();
	float height = y + part.getHeight();
	float max_width = math::max(width, getWidth());
	float max_height = math::max(height, getHeight());
	setTextureRect(sf::IntRect(0, 0, max_width, max_height));
}


//Todo, return NULL on fail (refactor api? ;))
Part* ComplexEntity::GetPartAt(size_t index)
{
	if (index < parts_.size())
	{
		return &parts_[index];
	}
	return NULL;
}

Part* ComplexEntity::GetPartByID(int id)
{
	for (size_t i = 0; i < parts_.size(); ++i)
	{
		if (parts_[i].GetID() == id)
		{
			return &parts_[i];
		}
	}
	return NULL;
}



int ComplexEntity::DestroyPart(int id)
{
	int nb_destroyed = 0;
	for (size_t i = 0; i < parts_.size(); ++i)
	{
		Part& part = parts_[i];
		if (part.GetID() == id && !part.IsDead())
		{
			part.Kill();
			++nb_destroyed;
		}
	}
	return nb_destroyed;
}


void ComplexEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	for (size_t i = 0; i < parts_.size(); ++i)
	{
		const Part& p = parts_[i];
		if (!p.IsDead())
			target.draw(parts_[i], states);
	}
}
