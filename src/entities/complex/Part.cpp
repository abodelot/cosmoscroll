#include "Part.hpp"
#define BLINK_DELAY 0.5f

#include <cstdio>

Part::Part(int id):
	Entity(sf::Vector2f(0, 0), 1)
{
	SetTeam(Entity::NEUTRAL);
	id_ = id;
	timer_ = 0;
}


void Part::Update(float frametime)
{
	if (timer_ >= 0)
	{
		timer_ -= frametime;
		int value = 255 - (255 * timer_ / BLINK_DELAY);
		SetColor(sf::Color(255, value, value));
		if (timer_ <= 0)
		{
			SetColor(sf::Color::White);
		}
	}
}


Entity* Part::Clone() const
{
	return new Part(*this);
}


int Part::GetID() const
{
	return id_;
}


void Part::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
	if (timer_ <= 0)
	{
		timer_ = BLINK_DELAY;
	}
}
