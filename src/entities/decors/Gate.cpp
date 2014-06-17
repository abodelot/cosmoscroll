#include "Gate.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"

#define DOOR_DELAY 2.f

#define ID_CELL  1
#define ID_BASE  2
#define ID_DOOR  3

Gate::Gate()
{
	Part cell(ID_CELL, 16);
	cell.setTexture(Resources::getTexture("entities/decor-energy-cell.png"));
	addPart(cell, 0, 28);

	Part base_top(ID_BASE);
	base_top.setTexture(Resources::getTexture("entities/decor-top.png"));
	base_top.setDestructible(false);
	addPart(base_top, 32);

	Part door(ID_DOOR);
	door.setTexture(Resources::getTexture("entities/decor-door.png"));
	door.setDestructible(false);
	addPart(door, 64, getHeight());
	door_full_height_ = door.getHeight();

	Part base_bottom(ID_BASE);
	base_bottom.setTexture(Resources::getTexture("entities/decor-bottom.png"));
	base_bottom.setDestructible(false);
	addPart(base_bottom, 32, getHeight());

	addPart(cell, 0, 332);

	energy_cells_ = 2;
	door_timer_ = 0;
}


void Gate::onUpdate(float frametime)
{
	move(-EntityManager::FOREGROUND_SPEED * frametime, 0.f);
	updateParts(frametime);

	if (door_timer_ > 0)
	{
		float delta_door = door_full_height_ * door_timer_ / DOOR_DELAY;
		Part* door = getPartByID(ID_DOOR);
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
	if (part.getID() == ID_CELL)
	{
		--energy_cells_;
		if (energy_cells_ == 0)
		{
			door_timer_ = DOOR_DELAY;
			SoundSystem::playSound("door-opening.ogg");
		}
	}
}
