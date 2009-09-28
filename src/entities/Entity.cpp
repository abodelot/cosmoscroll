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
	static const sf::FloatRect UNIVERSE(0, 0, WIN_WIDTH, GAME_HEIGHT);

	if (!UNIVERSE.Intersects(GetCollideRect()))
	{
		hp_ = 0;
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


bool Entity::PixelPerfectCollide() const
{
	return false;
}


bool Entity::IsCollidingWith(const Entity& other)
{
	if (PixelPerfectCollide() || other.PixelPerfectCollide())
	{
		sf::FloatRect r1, r2, overlap;
		GetCollideRect(r1);
		other.GetCollideRect(r2);

		// if overlapping rectangles
		if (r1.Intersects(r2, &overlap))
		{
			const int ALPHA = 0;

			int left1 = (int) overlap.Left - r1.Left;
			int top1 = (int) overlap.Top - r1.Top;

			int left2 = (int) overlap.Left - r2.Left;
			int top2 = (int) overlap.Top - r2.Top;

			int width = (int) overlap.GetWidth();
			int height = (int) overlap.GetHeight();

			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					if (GetPixel(x + left1, y + top1).a > ALPHA
						&& other.GetPixel(x + left2, y + top2).a > ALPHA)
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	// simple overlapping rectangles collision
	return GetCollideRect().Intersects(other.GetCollideRect());
}


Entity::Team Entity::GetTeam() const
{
	return team_;
}


void Entity::SetTeam(Team team)
{
	team_ = team;
}
