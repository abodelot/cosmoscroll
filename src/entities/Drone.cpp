#include "Drone.hpp"
#include "EntityManager.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"

#define GUN_OFFSET  sf::Vector2f(-8, 16)
#define SPEED	   200


Drone::Drone(const sf::Vector2f& offset, Entity* target) :
	Ennemy(offset, GET_IMG("ennemy-B"), 4, target),
	Animated(GET_ANIM("drone"), *this),
	weapon_(EntityManager::GetInstance().BuildWeapon(0))
{
	weapon_.SetOwner(this);
}


void Drone::Move(float frametime)
{
	sf::Sprite::Move(-SPEED * frametime, 0);
	KillIfOut();
	Animated::Update(frametime, *this);
	weapon_.Update(frametime);
}


void Drone::Action()
{
	float my_y = GetPosition().y;
	float player_y = target_->GetPosition().y;
	// Doit on tirer ?
	if (std::abs(player_y - my_y) < 30)
	{
		weapon_.Shoot(GetPosition() + GUN_OFFSET, -PI);
	}
}

