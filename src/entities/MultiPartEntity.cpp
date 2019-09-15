#include "MultiPartEntity.hpp"
#include "EntityManager.hpp"
#include "Explosion.hpp"
#include "core/Services.hpp"
#include "core/Collisions.hpp"


MultiPartEntity::MultiPartEntity()
{
    setTeam(Entity::NEUTRAL);
}


void MultiPartEntity::collides(Entity& entity)
{
    // Reverse it because elements drawn on top need to be checked first
    for (PartVector::reverse_iterator part = m_parts.rbegin(); part != m_parts.rend(); ++part) {
        if (!part->isDead()) {
            // HACK: the part's position is relative to the MultiPartEntity's position,
            // so we need to move the part to its absolute position to test the collision
            sf::Vector2f relative_pos = part->getPosition();
            part->move(getPosition());
            if (Services::getCollisions().pixelPerfectTest(entity, *part)) {
                // Restore original Part's position
                part->setPosition(relative_pos);
                entity.collides(*part);
                break;
            }
            part->setPosition(relative_pos);
        }
    }
}


void MultiPartEntity::updateParts(float frametime)
{
    for (size_t i = 0; i < m_parts.size(); ++i) {
        m_parts[i].onUpdate(frametime);
    }
}


void MultiPartEntity::addPart(Part& part, float x, float y)
{
    part.setPosition(x, y);
    part.setParent(this);
    part.setTeam(getTeam());
    m_parts.push_back(part);

    // The texture rect acts as a bounding box
    float right = x + part.getWidth();
    float bottom = y + part.getHeight();
    float width = std::max(right, getWidth());
    float height = std::max(bottom, getHeight());
    setTextureRect(sf::IntRect(0, 0, width, height));
}


MultiPartEntity::Part& MultiPartEntity::getPartAt(size_t index)
{
    return m_parts.at(index);
}


MultiPartEntity::Part* MultiPartEntity::getPartByID(int id)
{
    for (size_t i = 0; i < m_parts.size(); ++i) {
        if (m_parts[i].getID() == id) {
            return &m_parts[i];
        }
    }
    return nullptr;
}


int MultiPartEntity::destroyPartByID(int id)
{
    int nb_destroyed = 0;
    for (size_t i = 0; i < m_parts.size(); ++i) {
        Part& part = m_parts[i];
        if (part.getID() == id && part.getHP() > 0) {
            part.kill();
            ++nb_destroyed;
        }
    }
    return nb_destroyed;
}


void MultiPartEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    for (const Part& part: m_parts) {
        if (part.getHP() > 0) {
            target.draw(part, states);
        }
    }
}

// -----------------------------------------------------------------------------

MultiPartEntity::Part::Part(int id, int hp):
    m_id(id),
    m_destructible(true),
    m_parent(nullptr)
{
    setHP(hp);
}


void MultiPartEntity::Part::onUpdate(float frametime)
{
    updateDamageFlash(frametime);
}


void MultiPartEntity::Part::takeDamage(int damage)
{
    if (m_destructible) {
        Damageable::takeDamage(damage);
        m_parent->onPartDamaged(*this);
    }
}


int MultiPartEntity::Part::getID() const
{
    return m_id;
}


void MultiPartEntity::Part::setDestructible(bool destructible)
{
    m_destructible = destructible;
}


void MultiPartEntity::Part::setParent(MultiPartEntity* parent)
{
    m_parent = parent;
}


void MultiPartEntity::Part::onDestroy()
{
    m_parent->onPartDestroyed(*this);

    Explosion* explosion = new Explosion;
    sf::Vector2f pos = m_parent->getPosition() + getCenter();
    explosion->setPosition(pos);
    EntityManager::getInstance().addEntity(explosion);
}
