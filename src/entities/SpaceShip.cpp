#include "SpaceShip.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"
#include "../core/ParticleSystem.hpp"
#include "EntityManager.hpp"
#include "Bonus.hpp"


SpaceShip::SpaceShip(const char* animation, int hp, int speed) :
	Entity(GET_ANIM(animation).GetImage(), sf::Vector2f(0, 0), hp),
	Animated(GET_ANIM(animation), *this)
{
	speed_ = speed;
	weapon_ = NULL;
}


SpaceShip::~SpaceShip()
{
	if (weapon_ != NULL)
	{
		delete weapon_;
	}
}


SpaceShip* SpaceShip::Clone() const
{
	SpaceShip* ship = new SpaceShip(*this);
	if (weapon_ != NULL)
	{
		ship->SetWeapon(new Weapon(*weapon_));

	}

	return ship;
}


void SpaceShip::SetWeapon(Weapon* weapon)
{
	 weapon_ = weapon;
	 weapon->SetOwner(this);
}


void SpaceShip::Update(float frametime)
{
	sf::Sprite::Move(-speed_ * frametime, 0);
	KillIfOut();
	Animated::Update(frametime, *this);
	if (weapon_ != NULL)
	{
		weapon_->Update(frametime);
		/*float my_y = GetPosition().y;
		float player_y = target_->GetPosition().y;
		// Doit on tirer ?
		if (std::abs(player_y - my_y) < 30)
		{*/
			weapon_->Shoot(GetPosition(), -PI);
		//}
	}
}


void SpaceShip::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
	if (IsDead())
	{
		puts("space ship killed");
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


/** interceptor
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
