#include "Gate.hpp"
#include "entities/EntityManager.hpp"
#include "core/Resources.hpp"
#include "core/SoundSystem.hpp"

#define DOOR_DELAY 2.f

#define ID_CELL  1
#define ID_BASE  2
#define ID_DOOR  3

Gate::Gate():
    m_energy_cells_count(2),
    m_door_timer(0.f)
{
    Part cell(ID_CELL, 16);
    cell.setTexture(Resources::getTexture("entities/decor-energy-cell.png"));
    m_cell_animator1.setAnimation(cell, EntityManager::getInstance().getAnimation("energy-cell"));
    addPart(cell, 0, 28);

    Part base_top(ID_BASE);
    base_top.setTexture(Resources::getTexture("entities/decor-top.png"));
    base_top.setDestructible(false);
    addPart(base_top, 32);

    Part door(ID_DOOR);
    door.setTexture(Resources::getTexture("entities/decor-door.png"));
    door.setDestructible(false);
    addPart(door, 64, getHeight());
    m_door_height = door.getHeight();

    Part base_bottom(ID_BASE);
    base_bottom.setTexture(Resources::getTexture("entities/decor-bottom.png"));
    base_bottom.setDestructible(false);
    addPart(base_bottom, 32, getHeight());

    m_cell_animator2.setAnimation(cell, EntityManager::getInstance().getAnimation("energy-cell"));
    addPart(cell, 0, 332);
}


void Gate::onUpdate(float frametime)
{
    move(-EntityManager::FOREGROUND_SPEED * frametime, 0.f);
    m_cell_animator1.updateSubRect(getPartAt(0), frametime);
    m_cell_animator2.updateSubRect(getPartAt(4), frametime);
    updateParts(frametime);

    if (m_door_timer > 0)
    {
        float delta_door = m_door_height * m_door_timer / DOOR_DELAY;
        Part* door = getPartByID(ID_DOOR);
        sf::IntRect subrect(0, (m_door_height - delta_door), door->getWidth(), delta_door);
        m_door_timer -= frametime;
        if (m_door_timer <= 0)
        {
            subrect.top = m_door_height;
        }
        door->setTextureRect(subrect);
    }
}


void Gate::onPartDestroyed(const Part& part)
{
    if (part.getID() == ID_CELL)
    {
        --m_energy_cells_count;
        // Star open door timer when all cells are destroyed
        if (m_energy_cells_count == 0)
        {
            m_door_timer = DOOR_DELAY;
            SoundSystem::playSound("door-opening.ogg");
        }
    }
}
