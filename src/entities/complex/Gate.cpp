#include "Gate.hpp"
#include "Part.hpp"
#include "utils/MediaManager.hpp"

#define DOOR_DELAY 2.f

//TODO: remove magic numbers

Gate::Gate(const sf::Vector2f& pos):
	ComplexEntity(pos)
{
	const MediaManager& media = MediaManager::GetInstance();

	Part cell(1);
	cell.SetImage(media.GetImage("entities/decor-energy-cell"));
	cell.SetHP(10);
	AddPart(cell, 0, 4);
	AddPart(cell, 0, GetSize().y);
	AddPart(cell, 0, GetSize().y);

	Part base_top(2);
	base_top.SetImage(media.GetImage("entities/decor-base"));
	base_top.FlipY(true);
	base_top.SetHP(9000);
	AddPart(base_top, 32);

	Part door(3);
	door.SetImage(media.GetImage("entities/decor-door"));
	door.SetHP(9000);
	AddPart(door, 32 + 48, GetSize().y);
	door_full_height_ = door.GetSize().y;

	Part base_bottom(2);
	base_bottom.SetImage(media.GetImage("entities/decor-base"));
	base_bottom.SetHP(9000);
	AddPart(base_bottom, 32, GetSize().y);

	energy_cells_ = 3;
	door_timer_ = 0;
}


void Gate::Update(float frametime)
{
	sf::Sprite::Move(-60 * frametime, 0);
	if (door_timer_ > 0)
	{
		float door_height = door_full_height_ * door_timer_ / DOOR_DELAY;
		Part* door = GetPartByID(3);
		sf::IntRect subrect(0, 0, door->GetSize().x, door_height);
		door->SetSubRect(subrect);
		door_timer_ -= frametime;
	}
}


Entity* Gate::Clone() const
{
	return new Gate(*this);
}


void Gate::OnPartDestroyed(const Part& part)
{
	if (part.GetID() == 1)
	{
		--energy_cells_;
		if (energy_cells_ == 0)
		{
			door_timer_ = DOOR_DELAY;
		}
	}
}
