#include "Gate.hpp"
#include "Part.hpp"
#include "utils/Resources.hpp"

#define DOOR_DELAY 2.f

const sf::SoundBuffer& Gate::sound_opening_ = Resources::GetSoundBuffer("door-opening.ogg");

//TODO: remove magic numbers

Gate::Gate(const sf::Vector2f& pos):
	ComplexEntity(pos)
{
	Part cell(1);
	cell.SetImage(Resources::GetImage("entities/decor-energy-cell.png"));
	cell.SetHP(16);
	AddPart(cell, 0, 28);

	Part base_top(2);
	base_top.SetImage(Resources::GetImage("entities/decor-top.png"));
	base_top.SetCollideFlag(C_IGNORE_DAMAGE);
	AddPart(base_top, 32);

	Part door(3);
	door.SetImage(Resources::GetImage("entities/decor-door.png"));
	door.SetCollideFlag(C_IGNORE_DAMAGE);
	AddPart(door, 64, GetSize().y);
	door_full_height_ = door.GetSize().y;

	Part base_bottom(2);
	base_bottom.SetImage(Resources::GetImage("entities/decor-bottom.png"));
	base_bottom.SetCollideFlag(C_IGNORE_DAMAGE);
	AddPart(base_bottom, 32, GetSize().y);

	AddPart(cell, 0, 332);

	energy_cells_ = 2;
	door_timer_ = 0;
}


void Gate::Update(float frametime)
{
	ComplexEntity::Update(frametime);
	if (door_timer_ > 0)
	{
		float delta_door = door_full_height_ * door_timer_ / DOOR_DELAY;
		Part* door = GetPartByID(3);
		sf::IntRect subrect(0, (door_full_height_ - delta_door), door->GetSize().x, door_full_height_);
		door_timer_ -= frametime;
		if (door_timer_ <= 0)
		{
			subrect.Top = door_full_height_;
		}
		door->SetSubRect(subrect);
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
			SoundSystem::GetInstance().PlaySound(sound_opening_);
		}
	}
}
