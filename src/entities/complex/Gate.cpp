#include "Gate.hpp"
#include "Part.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"

#define DOOR_DELAY 2.f

#define CELL_ID  1
#define BASE_ID  2
#define DOOR_ID  3

Gate::Gate()
{
	Part cell(CELL_ID, 16);
	cell.setTexture(Resources::getTexture("entities/decor-energy-cell.png"));
	cell.setDestructible(true);
	addPart(cell, 0, 28);

	Part base_top(BASE_ID);
	base_top.setTexture(Resources::getTexture("entities/decor-top.png"));
	addPart(base_top, 32);

	Part door(DOOR_ID);
	door.setTexture(Resources::getTexture("entities/decor-door.png"));
	addPart(door, 64, getHeight());
	door_full_height_ = door.getHeight();

	Part base_bottom(BASE_ID);
	base_bottom.setTexture(Resources::getTexture("entities/decor-bottom.png"));
	addPart(base_bottom, 32, getHeight());

	addPart(cell, 0, 332);

	energy_cells_ = 2;
	door_timer_ = 0;
}


void Gate::onUpdate(float frametime)
{
	MultiPartEntity::onUpdate(frametime);
	if (door_timer_ > 0)
	{
		float delta_door = door_full_height_ * door_timer_ / DOOR_DELAY;
		Part* door = getPartByID(DOOR_ID);
		sf::IntRect subrect(0, (door_full_height_ - delta_door), door->getWidth(), delta_door);
		door_timer_ -= frametime;
		if (door_timer_ <= 0)
		{
			subrect.top = door_full_height_;
		}
		door->setTextureRect(subrect);
	}
}


void Gate::onPartDestroyed(const Part& part)
{
	if (part.getID() == CELL_ID)
	{
		--energy_cells_;
		if (energy_cells_ == 0)
		{
			door_timer_ = DOOR_DELAY;
			SoundSystem::GetInstance().PlaySound("door-opening.ogg");
		}
	}
}
