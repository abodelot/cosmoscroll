#include "Ennemy.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"
#include "../core/ParticleSystem.hpp"
#include "Bonus.hpp"
#include "../core/Game.hpp"

#include "Interceptor.hpp"
#include "Blorb.hpp"
#include "Drone.hpp"
#include "EvilBoss.hpp"

#include <SFML/System/Randomizer.hpp>


Ennemy::Ennemy(const sf::Vector2f& offset, const sf::Image& img, int hp,
	Entity* target) :
	Entity(img, offset, hp)
{
	target_ = target;
}


void Ennemy::Hit(int damage)
{
	Entity::Hit(damage);
	if (IsDead())
	{
#ifdef DEBUG
		if (sf::Randomizer::Random(0, 2) == 0)
#else
		if (sf::Randomizer::Random(0, 8) == 0)
#endif		
		{
			Game::GetInstance().AddEntity(Bonus::MakeRandom(GetPosition()));
		}
		ParticleSystem::GetInstance().AddExplosion(GetPosition());
	}
}


Ennemy* Ennemy::Make(Type type, const sf::Vector2f& offset, Entity* target)
{
	switch (type)
	{
		case BLORB:
			return new Blorb(offset, target);
		case DRONE:
			return new Drone(offset, target);
		case INTERCEPTOR:
			return new Interceptor(offset, target);
	}
	return NULL;
}

