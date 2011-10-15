#include "ComplexEntity.hpp"
#include "utils/Math.hpp"
#include "core/ParticleSystem.hpp"

ComplexEntity::ComplexEntity(const sf::Vector2f& pos):
	Entity(pos, 1)
{
	SetTeam(Entity::NEUTRAL);
	SetDamageable(false);
	SetCollideDamage(0);
}


void ComplexEntity::OnCollide(Entity& entity)
{
	for (PartVector::iterator it = parts_.begin(); it != parts_.end(); ++it)
	{
		sf::FloatRect entity_rect = entity.GetCollideRect();
		Part& p = *it;
		if (!p.IsDead())
		{
			// HACK: collide rect is relative to ComplexEntity position
			// thus, compute part actual position, and then set back relative position
			sf::Vector2f pos = p.GetPosition();
			p.Move(GetPosition());
			if (p.IsCollidingWith(entity, p.GetCollideRect(), entity_rect))
			{
				entity.OnCollide(p);
				p.OnCollide(entity);
				p.SetPosition(pos);
				if (p.IsDead())
				{
					OnPartDestroyed(p);
					sf::Vector2f pos;
					pos.x = GetPosition().x + p.GetPosition().x;
					pos.y = GetPosition().y + p.GetPosition().y;
					ParticleSystem::GetInstance().AddExplosion(pos);
				}
				break;
			}
			p.SetPosition(pos);
		}
	}
}


void ComplexEntity::AddPart(Part& part, float x, float y)
{
	part.SetPosition(x, y);
	//part.SetTeam(GetTeam());
	parts_.push_back(part);

	float width = x + part.GetSize().x;
	float height = y + part.GetSize().y;
	float max_width = math::max(width, GetSize().x);
	float max_height = math::max(height, GetSize().y);
	SetSubRect(sf::IntRect(0, 0, max_width, max_height));


	test_ = sf::Shape::Rectangle(0, 0, max_width, max_height, sf::Color(255, 0, 255, 64));
}


Part& ComplexEntity::GetPart(size_t index)
{
	if (index >= 0 && index < parts_.size())
	{
		return parts_[index];
	}
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


void ComplexEntity::Render(sf::RenderTarget& target) const
{
	for (size_t i = 0; i < parts_.size(); ++i)
	{
		const Part& p = parts_[i];
		if (!p.IsDead())
			target.Draw(parts_[i]);
	}
}
