#include "Asteroid.hpp"
#include "../utils/MediaManager.hpp"
#include "../core/ParticleSystem.hpp"
#include "EntityManager.hpp"
#include "../utils/Math.hpp"

#include <SFML/System.hpp>
#include <sstream>


// division en morceaux de taille moindre
#define BIG_SPLIT_INTO     2
#define MEDIUM_SPLIT_INTO  6


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


Asteroid::Asteroid(const sf::Vector2f& offset, Size size, float angle) :
    Entity(select_image(size), offset, size + 1)
{
    size_ = size;
    speed_ = 100;
    angle_ = DEG_TO_RAD(angle);
    //SetCenter(GetImage()->GetWidth() / 2, GetImage()->GetHeight() / 2);
}


void Asteroid::Update(float frametime)
{
    sf::Sprite::Move(-50 * frametime, 0); // mouvement latéral
/*
TODO
(00:08:43) Alexandre: manque un ajustement sur le facteur de ralentissement 0.99 appliqué à chaque frame
(00:09:21) Alexandre: donc l'astéro se prend N frames par seconde * 0.99 de ralentissement
(00:09:31) Alexandre: faut impliquer frametime dans le calcul pour que ce soit indépendant des FPS
(sinon plus on a de FPS plus l'astéro ralentit vite)
*/
    speed_ *= 0.99;
    float framespeed = frametime * speed_;
    sf::Vector2f offset = GetPosition();
    offset.x = offset.x + framespeed * math::cos(angle_);
    offset.y = offset.y - framespeed * math::sin(angle_);
    SetPosition(offset);
    //Rotate(90 * frametime);
    KillIfOut();
}


void Asteroid::TakeDamage(int damage)
{
	sf::Vector2f offset = GetPosition();

    Entity::TakeDamage(damage);
    if (IsDead())
    {
        EntityManager& entitymanager = EntityManager::GetInstance();
        switch (size_)
        {
            case BIG:
                for (int i = 0; i < BIG_SPLIT_INTO; ++i)
                {
                    entitymanager.AddEntity(new Asteroid(offset, MEDIUM,
                        sf::Randomizer::Random(0, 360)));
                };
                break;
            case MEDIUM:
                for (int i = 0; i < MEDIUM_SPLIT_INTO; ++i)
                {
                    entitymanager.AddEntity(new Asteroid(offset, SMALL,
                        sf::Randomizer::Random(0, 360)));
                }
                break;
            default:
                break;
        }
    }
}


