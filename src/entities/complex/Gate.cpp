#include "Gate.hpp"
#include "Part.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"

#define DOOR_DELAY 2.f


//TODO: remove magic numbers

Gate::Gate(const sf::Vector2f& pos):
	ComplexEntity(pos)
{
	Part cell(1);
	cell.setTexture(Resources::getTexture("entities/decor-energy-cell.png"));
	cell.setHP(16);
	cell.setDestructible(true);
	AddPart(cell, 0, 28);

	Part base_top(2);
	base_top.setTexture(Resources::getTexture("entities/decor-top.png"));
	AddPart(base_top, 32);

	Part door(3);
	door.setTexture(Resources::getTexture("entities/decor-door.png"));
	AddPart(door, 64, getHeight());
	door_full_height_ = door.getHeight();

	Part base_bottom(2);
	base_bottom.setTexture(Resources::getTexture("entities/decor-bottom.png"));
	AddPart(base_bottom, 32, getHeight());

	AddPart(cell, 0, 332);

	energy_cells_ = 2;
	door_timer_ = 0;
}


void Gate::onUpdate(float frametime)
{
	ComplexEntity::onUpdate(frametime);
	if (door_timer_ > 0)
	{
		float delta_door = door_full_height_ * door_timer_ / DOOR_DELAY;
		Part* door = GetPartByID(3);
		sf::IntRect subrect(0, (door_full_height_ - delta_door), door->getWidth(), delta_door);
		door_timer_ -= frametime;
		if (door_timer_ <= 0)
		{
			subrect.top = door_full_height_;
		}
		door->setTextureRect(subrect);
	}
}


Entity* Gate::clone() const
{
	return new Gate(*this);
}


void Gate::onPartDestroyed(const Part& part)
{
	if (part.GetID() == 1)
	{
		--energy_cells_;
		if (energy_cells_ == 0)
		{
			door_timer_ = DOOR_DELAY;
			SoundSystem::GetInstance().PlaySound("door-opening.ogg");
		}
	}
}
