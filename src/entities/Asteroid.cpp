#include <SFML/System.hpp>

#include "Asteroid.hpp"
#include "EntityManager.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/StringUtils.hpp"
#include "../utils/Math.hpp"

// division en morceaux de taille moindre
#define BIG_SPLIT_INTO     2
#define MEDIUM_SPLIT_INTO  4
#define SLOWDOWN           0.99f
#define SCROLL_SPEED       50


Asteroid::Asteroid(const sf::Vector2f& offset, Size size, float angle) :
	Entity(offset, size + 1)
{
	speed_ = 100;
	angle_ = math::deg_to_rad(angle);
	size_ = size;
	SetRandomImage();
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
	Asteroid* asteroid = new Asteroid(*this);
	asteroid->SetRandomImage();
	return asteroid;
}


void Asteroid::Update(float frametime)
{
	sf::Sprite::Move(-SCROLL_SPEED * frametime, 0);
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


void Asteroid::SetRandomImage()
{
	int x = 0;
	switch (size_)
	{
		case SMALL:
			SetImage(GET_IMG("entities/asteroid-small"));
			x = sf::Randomizer::Random(0, 3) * 16;
			SetSubRect(sf::IntRect(x, 0, x + 16, 16));
			break;
		case MEDIUM:
			SetImage(GET_IMG("entities/asteroid-medium"));
			x = sf::Randomizer::Random(0, 2) * 32;
			SetSubRect(sf::IntRect(x, 0, x + 32, 32));
			break;
		case BIG:
			SetImage(GET_IMG("entities/asteroid-big"));
			x = sf::Randomizer::Random(0, 2) * 48;
			SetSubRect(sf::IntRect(x, 0, x + 48, 48));
			break;
	}
}


