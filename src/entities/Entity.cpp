#include "Entity.hpp"
#include "EntityManager.hpp"


Entity::Entity(const sf::Vector2f& position, int hp, int collide_damage)
{
	SetPosition(position);
	team_ = NEUTRAL;
	hp_ = hp;
	flipped_ = false;
	collide_damage_ = collide_damage;
	points_ = 0;
	damageable_ = true;
}


Entity::~Entity()
{
}


sf::Vector2f Entity::GetCenter_() const
{
	sf::Vector2f pos = GetPosition();
	pos.x += GetSubRect().GetWidth() / 2;
	pos.y += GetSubRect().GetHeight() / 2;
	return pos;
}


void Entity::SetTarget(Entity*)
{
}


void Entity::TakeDamage(int damage)
{
	if (damageable_)
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


int Entity::GetHP() const
{
	return hp_;
}

void Entity::KillIfOut()
{
	static EntityManager& entity_mgr = EntityManager::GetInstance();
	sf::FloatRect rect = GetCollideRect();
	if (rect.Bottom < 0
	 || rect.Top    > entity_mgr.GetHeight()
	 || rect.Right  < 0
	 || rect.Left   > entity_mgr.GetWidth())
	{
		hp_ = 0;
	}
}


sf::FloatRect Entity::GetCollideRect() const
{
	const sf::Vector2f& pos = GetPosition();
	const sf::Vector2f& origin = GetCenter();

	sf::FloatRect rect;
	rect.Left = pos.x - origin.x;
	rect.Top = pos.y - origin.y;
	rect.Right = rect.Left + GetSize().x;
	rect.Bottom = rect.Top + GetSize().y;
	return rect;
}


bool Entity::PixelPerfectCollide() const
{
	return false;
}


#define ALPHACOMP(buf, width, x, y) (buf[((x) + (y) * (width)) * 4 + 3])
/* gets to the alpha component of pixelsPtr[x,y] (Picture width being provided)*/

bool Entity::IsCollidingWith(const Entity& other)
{
	if (((this->GetImage() && other.GetImage()))
		&& (PixelPerfectCollide() || other.PixelPerfectCollide()))
	{
		sf::FloatRect r1 = GetCollideRect();
		sf::FloatRect r2 = other.GetCollideRect();
		sf::FloatRect overlap;

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

			const sf::Uint8 *myPix, *otherPix;
			myPix = this->GetImage()->GetPixelsPtr();
			otherPix = other.GetImage()->GetPixelsPtr();
			int myWidth = this->GetImage()->GetWidth();
			int otherWidth = other.GetImage()->GetWidth();
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					if (ALPHACOMP(myPix, myWidth, (x + left1), (y + top1)) > ALPHA &&
						ALPHACOMP(otherPix, otherWidth, (x + left2), (y + top2)) > ALPHA)
					{
						return true;
					}
					/*if (GetPixel(x + left1, y + top1).a > ALPHA
						&& other.GetPixel(x + left2, y + top2).a > ALPHA)
					{
						return true;								
					}*/
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


void Entity::SetDamageable(bool damageable)
{
	damageable_ = damageable;
}
