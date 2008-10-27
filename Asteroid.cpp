#include "Asteroid.hpp"
#include "MediaManager.hpp"
#include "ParticleSystem.hpp"
#include "Game.hpp"
#include "Math.hpp"
#include "Window.hpp"

#include <SFML/System.hpp>
#include <sstream>

// orientation en degrés
#define MIN_ANGLE 150.0f
#define MAX_ANGLE 200.0f

// vitesse en pixels/seconde
#define MIN_SPEED  20.0f
#define MAX_SPEED  40.0f

// division en morceaux de taille moindre
#define BIG_SPLIT_INTO     2
#define MEDIUM_SPLIT_INTO  3


inline const sf::Image& select_image(Asteroid::Size size)
{
    const int NB = 3;
    static const char* image_bases[] = {
        "asteroid-small",
        "asteroid-medium",
        "asteroid-big"
    };
    std::ostringstream key;
    key << image_bases[size] << '-' << sf::Randomizer::Random(1, NB);
    return GET_IMG(key.str().c_str());
}


Asteroid::Asteroid(const sf::Vector2f& offset, Size size) :
    Entity(select_image(size), offset, size + 1)
{
    size_ = size;
    speed_ = sf::Randomizer::Random(MIN_SPEED, MAX_SPEED);
    angle_ = DEG_TO_RAD(sf::Randomizer::Random(MIN_ANGLE, MAX_ANGLE));
}


void Asteroid::Action()
{
    // TODO: rien >__>
}


void Asteroid::Move(float frametime)
{
    float framespeed = frametime * speed_;
    sf::Vector2f offset = sprite_.GetPosition();
    offset.x = offset.x + framespeed * std::cos(angle_);
    offset.y = offset.y - framespeed * std::sin(angle_);
    sprite_.SetPosition(offset);
    if (outside_universe(GetRect()))
    {
        Kill();
    }
}


void Asteroid::Hit(int damage)
{
    ParticleSystem& p = ParticleSystem::GetInstance();
    sf::Vector2f offset = sprite_.GetPosition();
    
    Entity::Hit(damage);
    if (IsDead())
    {
        Asteroid* as = NULL;
        switch (size_)
        {
            case BIG:
                for (int i = 0; i < BIG_SPLIT_INTO; ++i)
                {
                    as = new Asteroid(offset, MEDIUM);
                    Game::GetInstance().AddEntity(as);
                };
                p.AddImpact(offset, 20);
                break;
            case MEDIUM:
                for (int i = 0; i < MEDIUM_SPLIT_INTO; ++i)
                {
                    as = new Asteroid(offset, SMALL);
                    Game::GetInstance().AddEntity(as);
                }
                p.AddImpact(offset, 10);
            case SMALL:
                p.AddImpact(offset, 5);
                break;
        }
    }
}

