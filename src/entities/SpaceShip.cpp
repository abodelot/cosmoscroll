#include <cstring>

#include "SpaceShip.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"
#include "../utils/StringUtils.hpp"
#include "../core/ParticleSystem.hpp"
#include "EntityManager.hpp"
#include "Bonus.hpp"

// bonus freq = 1 / DROP_LUCK
#define DROP_LUCK 8

#define DEFAULT_MOVE_PATTERN   &SpaceShip::MP_STRAIGHT;
#define DEFAULT_ATTACK_PATTERN &SpaceShip::AP_NO_ATTACK;

#define TEST_MOVE(pattern, test, ptr) \
	if (strcmp(pattern, #test) == 0)\
	{\
		ptr = &SpaceShip::MP_##test;\
		return;\
	}

#define TEST_ATTACK(pattern, test, ptr) \
	if (strcmp(pattern, #test) == 0)\
	{\
		ptr = &SpaceShip::AP_##test;\
		return;\
	}



SpaceShip::SpaceShip(const char* animation, int hp, int speed) :
	Entity(sf::Vector2f(0, 0), hp),
	Animated(EntityManager::GetInstance().GetAnimation(animation), *this)
{
	SetTeam(Entity::BAD);

	move_pattern_ = DEFAULT_MOVE_PATTERN;
	attack_pattern_ = DEFAULT_ATTACK_PATTERN;
	speed_ = speed;
	weapon_.SetOwner(this);
	target_ = NULL;
}


SpaceShip::~SpaceShip()
{
}


void SpaceShip::SetMovePattern(const char* pattern)
{
	if (pattern == NULL)
		return;

	TEST_MOVE(pattern, STRAIGHT, move_pattern_)
	TEST_MOVE(pattern, MAGNET, move_pattern_)

	printf("error: undefined move pattern: %s\n", pattern);
}


void SpaceShip::SetAttackPattern(const char* pattern)
{
	if (pattern == NULL)
		return;

	TEST_ATTACK(pattern, AUTO_AIM, attack_pattern_)
	TEST_ATTACK(pattern, ON_SIGHT, attack_pattern_)
	TEST_ATTACK(pattern, NO_ATTACK, attack_pattern_)

	printf("error: undefined attack pattern: %s\n", pattern);
}


Weapon* SpaceShip::GetWeapon()
{
	return &weapon_;
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


void SpaceShip::Update(float frametime)
{
	(this->*move_pattern_)(frametime);
	(this->*attack_pattern_)();

	Animated::Update(frametime, *this);
	weapon_.Update(frametime);
	KillIfOut();
}


void SpaceShip::TakeDamage(int damage)
{
	if (!IsDead())
	{
		Entity::TakeDamage(damage);
		if (IsDead())
		{
			if (sf::Randomizer::Random(1, DROP_LUCK) == 1)
			{
				EntityManager::GetInstance().AddEntity(Bonus::MakeRandom(GetPosition()));
			}
			ParticleSystem::GetInstance().AddExplosion(GetPosition());
		}
	}
}

// movement patterns -----------------------------------------------------------

void SpaceShip::MP_MAGNET(float frametime)
{
	float velocity = speed_ * frametime;
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

	if (flipped != IsFlipped())
	{
		Flip(flipped);
	}

	sf::Sprite::Move(vx, vy);
}


void SpaceShip::MP_STRAIGHT(float frametime)
{
	sf::Sprite::Move(-speed_ * frametime, 0);
}

// attack patterns -------------------------------------------------------------

void SpaceShip::AP_AUTO_AIM()
{
	float radians = math::angle(target_->GetPosition(), GetPosition());

	if (IsFlipped())
	{
		sf::Vector2f pos = GetPosition();
		pos.x += GetSize().x;
		weapon_.Shoot(pos, radians);
	}
	else
	{
		weapon_.Shoot(GetPosition(), radians);
	}
}


void SpaceShip::AP_ON_SIGHT()
{
	float my_y = GetPosition().y;
	float player_y = target_->GetPosition().y;
	// Doit on tirer ?
	if (std::abs(player_y - my_y) < 30)
	{
		weapon_.Shoot(GetPosition(), -PI);
	}
}


void SpaceShip::AP_NO_ATTACK()
{
}
