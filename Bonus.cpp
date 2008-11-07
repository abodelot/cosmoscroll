#include "Bonus.hpp"
#include "MediaManager.hpp"

#include <SFML/System/Randomizer.hpp>

#define SPEED 100


static inline const sf::Image& select_image(Bonus::Type type)
{
    static const char* keys[Bonus::BONUS_COUNT] = {
        "bonus-health",
        "bonus-trigun",
		"bonus-cooler"
    };
    return GET_IMG(keys[type]);
}


Bonus::Bonus(Type type, const sf::Vector2f& offset) :
    Entity(select_image(type), offset, 1)
{
    type_ = type;
}


Bonus* Bonus::MakeRandom(const sf::Vector2f& offset)
{
    Type type = (Type) sf::Randomizer::Random(0, BONUS_COUNT - 1);
    return new Bonus(type, offset);
}


void Bonus::Hit(int damage)
{
    (void) damage; // un bonus ne peut être détruit
}


void Bonus::Move(float frametime)
{
    sprite_.Move(-SPEED * frametime, 0);
}

