#include <cstring>

#include "SpaceShip.hpp"
#include "EntityManager.hpp"
#include "Bonus.hpp"

#include "utils/Math.hpp"
#include "utils/MediaManager.hpp"
#include "utils/StringUtils.hpp"
#include "core/ParticleSystem.hpp"


// bonus freq = 1 / DROP_LUCK
#define DROP_LUCK 8

#define DEFAULT_MOVE_PATTERN   &SpaceShip::move_straight;
#define DEFAULT_ATTACK_PATTERN &SpaceShip::attack_none;

#define TEST_MOVE(pattern, test, ptr) \
	if (strcmp(pattern, #test) == 0)\
	{\
		ptr = &SpaceShip::move_##test;\
		return;\
	}

#define TEST_ATTACK(pattern, test, ptr) \
	if (strcmp(pattern, #test) == 0)\
	{\
		ptr = &SpaceShip::attack_##test;\
		return;\
	}

// paramètres de la fonction de déplacement sinus
#define SINUS_AMPLITUDE 60
#define SINUS_FREQUENCE 0.02


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
	base_y_ = -1;
}


SpaceShip::~SpaceShip()
{
}


void SpaceShip::SetMovePattern(const char* pattern)
{
	if (pattern == NULL)
		return;

	TEST_MOVE(pattern, straight, move_pattern_)
	TEST_MOVE(pattern, magnet, move_pattern_)
	TEST_MOVE(pattern, sinus, move_pattern_)
	fprintf(stderr, "error: undefined move pattern: %s\n", pattern);
}


void SpaceShip::SetAttackPattern(const char* pattern)
{
	if (pattern == NULL)
		return;

	TEST_ATTACK(pattern, auto_aim, attack_pattern_)
	TEST_ATTACK(pattern, on_sight, attack_pattern_)
	TEST_ATTACK(pattern, none, attack_pattern_)

	fprintf(stderr, "error: undefined attack pattern: %s\n", pattern);
}


Weapon* SpaceShip::GetWeapon()
{
	return &weapon_;
}


SpaceShip* SpaceShip::Clone() const
{
	SpaceShip* ship = new SpaceShip(*this);
	ship->SetPoints(GetPoints());
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

void SpaceShip::move_magnet(float frametime)
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
	KillIfOut();
}


void SpaceShip::move_straight(float frametime)
{
	sf::Sprite::Move(-speed_ * frametime, 0);
	KillIfOut();
}


void SpaceShip::move_sinus(float frametime)
{
	sf::Vector2f pos = GetPosition();
	pos.x += -speed_ * frametime;
	if (base_y_ == -1)
	{
		int y_max = EntityManager::GetInstance().GetHeight() - SINUS_AMPLITUDE - GetSize().y;
		// calcul de l'ordonnée à l'origine base_y_ de la fonction sinus
		// l'amplitude de la courbe ne doit pas sortir de la zone de jeu
		if (pos.y < SINUS_AMPLITUDE)
		{
			base_y_ = SINUS_AMPLITUDE;
		}
		else if (pos.y > y_max)
		{
			base_y_ = y_max;
		}
		else
		{
			base_y_ = pos.y;
		}
	}
	pos.y = std::sin(pos.x * SINUS_FREQUENCE) * SINUS_AMPLITUDE + base_y_;
	SetPosition(pos);
	if (pos.x < -GetSize().x)
	{
		Kill();
	}
}

// attack patterns -------------------------------------------------------------

void SpaceShip::attack_auto_aim()
{
	weapon_.ShootAt(target_->GetPosition());
}


void SpaceShip::attack_on_sight()
{
	float my_y = GetPosition().y;
	float player_y = target_->GetPosition().y;
	// Doit on tirer ?
	if (std::abs(player_y - my_y) < 30)
	{
		weapon_.Shoot(-PI);
	}
}


void SpaceShip::attack_none()
{
}
