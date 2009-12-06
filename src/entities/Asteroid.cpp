#include <SFML/System.hpp>
#include <sstream>

#include "Asteroid.hpp"
#include "EntityManager.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"

// division en morceaux de taille moindre
#define BIG_SPLIT_INTO     2
#define MEDIUM_SPLIT_INTO  6
#define SLOWDOWN           0.99f
#define VERTICAL_SPEED     50


Asteroid::Asteroid(const sf::Vector2f& offset, Size size, float angle) :
	Entity(offset, size + 1)
{
	const int NB = 3;
	static const char* image_bases[] = {
		"asteroid-small",
		"asteroid-medium",
		"asteroid-big"
	};
	std::ostringstream key;
	key << image_bases[size] << '-' << sf::Randomizer::Random(1, NB);
	SetImage(GET_IMG(key.str().c_str()));

	speed_ = 100;
	angle_ = math::deg_to_rad(angle);

	size_ = size;
	/*SetCenter(GetImage()->GetWidth() / 2, GetImage()->GetHeight() / 2);
	static const sf::IntRect rect_big[COUNT_BIG] = {
		sf::IntRect(),
		sf::IntRect(),
		sf::IntRect()
	};
	static const sf::IntRect rect_medium[COUNT_MEDIUM] = {
		sf::IntRect(),
		sf::IntRect(),
		sf::IntRect()
	};
	static const sf::IntRect rect_small[COUNT_SMALL] = {
		sf::IntRect(),
		sf::IntRect(),
		sf::IntRect(),
		sf::IntRect()
	}
	switch (size)
	{
		case SMALL:
			SetSubRect(rect_small[sf::Randomizer::Random(0, COUNT_SMALL - 1)]);
			break;
		case MEDIUM:
			SetSubRect(rect_medium[sf::Randomizer::Random(0, COUNT_MEDIUM - 1)]);
			break;
		case BIG:
			SetSubRect(rect_big[sf::Randomizer::Random(0, COUNT_BIG - 1)]);
			break;
	}*/
}


Asteroid* Asteroid::Clone() const
{
	return new Asteroid(*this);
}


void Asteroid::Update(float frametime)
{
	sf::Sprite::Move(-VERTICAL_SPEED * frametime, 0);
/*
TODO
(00:08:43) Alexandre: manque un ajustement sur le facteur de ralentissement 0.99 appliqué à chaque frame
(00:09:21) Alexandre: donc l'astéro se prend N frames par seconde * 0.99 de ralentissement
(00:09:31) Alexandre: faut impliquer frametime dans le calcul pour que ce soit indépendant des FPS
(sinon plus on a de FPS plus l'astéro ralentit vite)
*/
	speed_ *= SLOWDOWN;
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
		static EntityManager& entitymanager = EntityManager::GetInstance();
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

