#include "Entity.hpp"
#define FLASH_DELAY 0.5f


Entity::Entity(const sf::Vector2f& position, int hp, int collide_damage)
{
	SetPosition(position);
	team_ = NEUTRAL;
	hp_ = hp;
	flipped_x_ = false;
	flipped_y_ = false;
	collide_damage_ = collide_damage;
	collide_flag_ = 0;
	points_ = 0;
	flash_timer_ = 0.f;
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


void Entity::FlipX(bool flip)
{
	sf::Sprite::FlipX(flip);
	flipped_x_ = flip;
}


void Entity::FlipY(bool flip)
{
	sf::Sprite::FlipY(flip);
	flipped_y_ = flip;
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


#define ALPHACOMP(buf, width, x, y) (buf[((x) + (y) * (width)) * 4 + 3])
/* gets to the alpha component of pixelsPtr[x,y] (Picture width being provided)*/

bool Entity::IsCollidingWith(const Entity& other, const sf::FloatRect& r1, const sf::FloatRect& r2)
{
	sf::FloatRect overlap;

	// if overlapping rectangles
	if (r1.Intersects(r2, &overlap))
	{
		if (!this->GetImage() || !other.GetImage())
		{
			return true;
		}
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
		if (this->flipped_x_|| other.flipped_x_ || this->flipped_y_ || other.flipped_y_)
		{
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
		else
		{
			for (int y = 0; y < height; ++y)
			{
				for (int x = 0; x < width; ++x)
				{
					if (ALPHACOMP(myPix, myWidth, (x + left1), (y + top1)) > ALPHA &&
						ALPHACOMP(otherPix, otherWidth, (x + left2), (y + top2)) > ALPHA)
					{
						return true;
					}
				}
			}
		}
	}
	return false;
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
		SetColor(sf::Color(255, value, value));
		if (flash_timer_ <= 0)
		{
			SetColor(sf::Color::White);
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
