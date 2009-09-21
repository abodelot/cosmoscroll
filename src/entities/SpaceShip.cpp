#include "SpaceShip.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"
#include "../core/ParticleSystem.hpp"
#include "EntityManager.hpp"
#include "Bonus.hpp"


SpaceShip::SpaceShip(const char* animation, int hp, int speed) :
	Entity(sf::Vector2f(0, 0), hp),
	Animated(EntityManager::GetInstance().GetAnimation(animation), *this)
{
	SetTeam(Entity::BAD);

	speed_ = speed;
	weapon_.SetOwner(this);
	target_ = NULL;
}


SpaceShip::~SpaceShip()
{
}


SpaceShip* SpaceShip::Clone() const
{
	SpaceShip* ship = new SpaceShip(*this);
	ship->GetWeapon()->SetOwner(ship);
	return ship;
}


void SpaceShip::SetTarget(Entity* target)
{
	target_ = target;
}


Weapon* SpaceShip::GetWeapon()
{
	return &weapon_;
}


void SpaceShip::Update(float frametime)
{
	sf::Sprite::Move(-speed_ * frametime, 0);
	KillIfOut();
	Animated::Update(frametime, *this);
	if (weapon_.IsInited())
	{
		weapon_.Update(frametime);
		if (target_ == NULL)
		{
			weapon_.Shoot(GetPosition(), -PI);
		}
		else
		{
			float my_y = GetPosition().y;
			float player_y = target_->GetPosition().y;
			// Doit on tirer ?
			if (std::abs(player_y - my_y) < 30)
			{
				weapon_.Shoot(GetPosition(), -PI);
			}
		}
	}
}


void SpaceShip::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
	if (IsDead())
	{
#ifdef DEBUG
		if (sf::Randomizer::Random(0, 2) == 0)
#else
		if (sf::Randomizer::Random(0, 8) == 0)
#endif
		{
			EntityManager::GetInstance().AddEntity(Bonus::MakeRandom(GetPosition()));
		}
		ParticleSystem::GetInstance().AddExplosion(GetPosition());
	}
}


/* interceptor
tir:
 float radians = ANGLE(target_->GetPosition(), GetPosition());
    if (flipped_)
    {
        weapon_.Shoot(GetPosition() + GUN_OFFSET_INVERT, radians);
    }
    else
    {
        weapon_.Shoot(GetPosition() + GUN_OFFSET, radians);
    }

move:
float velocity = SHIP_SPEED * frametime;
    float vy = 0;
    float vx = 0;
    sf::Vector2f player_pos = target_->GetPosition();
    sf::Vector2f my_pos = GetPosition();

    bool flipped = false;
    if (my_pos.x > player_pos.x)
    {
    	vx = -velocity;
    }
    else if (my_pos.x < player_pos.x)
    {
    	vx = velocity;
    	flipped = true;
    }

	if (my_pos.y > player_pos.y)
	    vy = -velocity;
	else if (my_pos.y < player_pos.y)
	    vy = velocity;

	if (flipped != flipped_)
	{
	    Flip(flipped);
	}

    sf::Sprite::Move(vx, vy);
    weapon_.Update(frametime);
*/


/* scoot/

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

*/
