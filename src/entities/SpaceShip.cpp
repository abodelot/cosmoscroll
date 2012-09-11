#include <cstring>
#include <cstdio>
#include<cmath>

#include "SpaceShip.hpp"
#include "EntityManager.hpp"
#include "Bonus.hpp"

#include "utils/Math.hpp"
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


SpaceShip::SpaceShip(const Animation& animation, int hp, int speed) :
	Entity(sf::Vector2f(0, 0), hp),
	Animated(animation)
{
	setTexture(animation.getTexture());
	SetTeam(Entity::BAD);
	Reset(*this);

	move_pattern_ = DEFAULT_MOVE_PATTERN;
	attack_pattern_ = DEFAULT_ATTACK_PATTERN;
	speed_ = speed;
	weapon_.SetOwner(this);
	target_ = NULL;
	base_y_ = -1;
	base_x_ = -1;
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
	TEST_MOVE(pattern,circle,move_pattern_)
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

	Animated::UpdateSubRect(*this, frametime);
	weapon_.Update(frametime);
	Entity::UpdateFlash(frametime);
}


void SpaceShip::OnDestroy()
{
	if (math::random(1, DROP_LUCK) == 1)
	{
		EntityManager::GetInstance().AddEntity(Bonus::MakeRandom(getPosition()));
	}
	ParticleSystem::GetInstance().ExplosionSfx(getCenter());
}

// movement patterns -----------------------------------------------------------

void SpaceShip::move_circle(float frametime)
{

    float current_angle;
    sf::Vector2f pos = getPosition();

   if(pos.x > 580 )
        pos.x -= speed_*frametime;
    else
    {
            //Record Circle's Centre at Entry
            if(base_y_ == -1)
            {
                base_y_ =  (int)pos.y;
                base_x_ =  pos.x - 30;
                current_angle = 0;
            }
            else
            {
                if(pos.y== base_y_  && pos.x > base_x_ )
                    current_angle += math::PI/128;//0+inc.
                else if(pos.x == base_x_ && pos.y < base_y_)
                    current_angle = math::PI/2+math::PI/128;
                else if(pos.y == base_y_ && pos.x < base_x_ )
                    current_angle = math::PI +  math::PI/128;
                else if(pos.x == base_x_ && pos.y > base_y_)
                    current_angle = 3*math::PI/2 + math::PI/128;
                else
                    current_angle = atan2( ( base_y_ - pos.y) , (pos.x - base_x_) )+math::PI/128;

                pos.x = base_x_ + 30*std::cos(current_angle) ;
                pos.y = base_y_ - 30*std::sin(current_angle);
            }
    }
    setPosition(pos);
}


void SpaceShip::move_magnet(float frametime)
{
	sf::Vector2f player_pos = target_->getCenter();
	sf::Vector2f my_pos = getCenter();

	float velocity = speed_ * frametime;
	float x_diff = my_pos.x - player_pos.x;
	float y_diff = my_pos.y - player_pos.y;

	if (x_diff > 5)
		sf::Sprite::move(-velocity, 0);
	else if (x_diff < -5)
		sf::Sprite::move(velocity, 0);

	if (y_diff > 5)
		sf::Sprite::move(0, -velocity);
	else if (y_diff < -5)
		sf::Sprite::move(0, velocity);
}


void SpaceShip::move_straight(float frametime)
{
	sf::Sprite::move(-speed_ * frametime, 0);
}


void SpaceShip::move_sinus(float frametime)
{
	sf::Vector2f pos = getPosition();
	pos.x += -speed_ * frametime;
	if (base_y_ == -1)
	{
		int y_max = EntityManager::GetInstance().GetHeight() - SINUS_AMPLITUDE - getHeight();
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
	setPosition(pos);
}

// attack patterns -------------------------------------------------------------

void SpaceShip::attack_auto_aim()
{
	weapon_.ShootAt(target_->getCenter());
}


void SpaceShip::attack_on_sight()
{
	float my_y = getCenter().y;
	float player_y = target_->getCenter().y;
	// Doit on tirer ?
	if (std::abs(player_y - my_y) < 30)
	{
		weapon_.Shoot(-math::PI);
	}
}


void SpaceShip::attack_none()
{
}
