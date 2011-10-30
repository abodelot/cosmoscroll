#include "Part.hpp"


Part::Part(int id):
	Entity(sf::Vector2f(0, 0), 1)
{
	SetTeam(Entity::NEUTRAL);
	id_ = id;
}


void Part::Update(float frametime)
{
	Entity::UpdateFlash(frametime);
}


Entity* Part::Clone() const
{
	return new Part(*this);
}


int Part::GetID() const
{
	return id_;
}
