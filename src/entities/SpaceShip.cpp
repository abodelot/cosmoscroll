#include "SpaceShip.hpp"
#include "EntityManager.hpp"
#include "Bonus.hpp"

#include "utils/Math.hpp"
#include "utils/StringUtils.hpp"
#include "core/ParticleSystem.hpp"


// bonus freq = 1 / DROP_LUCK
#define DROP_LUCK 8

// sinus movement settings
#define SINUS_AMPLITUDE 60
#define SINUS_FREQUENCE 0.02

// circle movement settings
#define CIRCLE_START_AT 400
#define CIRCLE_RADIUS   60
#define CIRCLE_ROTATION_SPEED (math::PI * 0.8)

SpaceShip::SpaceShip(const Animation& animation, int hp, int speed) :
	Entity(sf::Vector2f(0, 0), hp),
	attack_(NONE),
	movement_(LINE)
{
	setTexture(animation.getTexture());
	SetTeam(Entity::BAD);
	animator_.setAnimation(*this, animation);

	speed_ = speed;
	weapon_.setOwner(this);
	target_ = NULL;
	base_y_ = 0;
	base_x_ = 0;
	angle_ = 2 * math::PI;
}


SpaceShip::~SpaceShip()
{
}


void SpaceShip::onInit()
{
	if (movement_ == SINUS)
	{
		float y = getY();
		// Compute maximal Y position according to the wave's amplitude
		// (So the spacship is always within the screen's boundaries)
		float y_max = EntityManager::GetInstance().GetHeight() - SINUS_AMPLITUDE - getHeight();
		if (y < SINUS_AMPLITUDE)
			base_y_ = SINUS_AMPLITUDE;
		else if (y > y_max)
			base_y_ = y_max;
		else
			base_y_ = y;

		setY(base_y_);
	}
	else if (movement_ == CIRCLE)
	{
		float y = getY();
		float y_max = EntityManager::GetInstance().GetHeight() - CIRCLE_RADIUS - getHeight();
		if (y < CIRCLE_RADIUS)
			base_y_ = CIRCLE_RADIUS;
		else if (y > y_max)
			base_y_ = y_max;
		else
			base_y_ = y;

		setY(base_y_);
	}
}


void SpaceShip::setMovementPattern(MovementPattern movement)
{
	movement_ = movement;
}


void SpaceShip::setAttackPattern(AttackPattern attack)
{
	attack_ = attack;
}


SpaceShip* SpaceShip::Clone() const
{
	SpaceShip* ship = new SpaceShip(*this);
	ship->SetPoints(GetPoints());
	ship->weapon_.setOwner(ship);
	return ship;
}


void SpaceShip::SetTarget(Entity* target)
{
	target_ = target;
}


void SpaceShip::Update(float frametime)
{
	// Apply movement pattern
	float delta = speed_ * frametime;
	switch (movement_)
	{
		case LINE:
		{
			move(-delta, 0);
		}
		break;
		case MAGNET:
		{
			sf::Vector2f player_pos = target_->getCenter();
			sf::Vector2f my_pos = getCenter();

			float x_diff = my_pos.x - player_pos.x;
			float y_diff = my_pos.y - player_pos.y;

			if (x_diff > 5)
				move(-delta, 0);
			else if (x_diff < -5)
				move(delta, 0);

			if (y_diff > 5)
				move(0, -delta);
			else if (y_diff < -5)
				move(0, delta);
		}
		break;
		case SINUS:
		{
			sf::Vector2f pos = getPosition();
			pos.x += -speed_ * frametime;
			pos.y = std::sin(pos.x * SINUS_FREQUENCE) * SINUS_AMPLITUDE + base_y_;
			setPosition(pos);
		}
		break;
		case CIRCLE:
		{
			if (getX() > CIRCLE_START_AT + CIRCLE_RADIUS)
			{
				move(-delta, 0);
			}
			else
			{
				sf::Vector2f pos(CIRCLE_START_AT, base_y_);
				angle_ += CIRCLE_ROTATION_SPEED * frametime;
				pos.x += CIRCLE_RADIUS * std::cos(angle_);
				pos.y -= CIRCLE_RADIUS * std::sin(angle_);
				setPosition(pos);
			}
		}
		break;
	}


	switch (attack_)
	{
		case AUTO_AIM:
			weapon_.shoot(target_->getCenter());
			break;

		case ON_SIGHT:
		{
			float my_y = getCenter().y;
			float player_y = target_->getCenter().y;
			// if both spaceships are roughly on the same Y axis
			if (std::abs(player_y - my_y) < 30)
			{
				weapon_.shoot(-math::PI);
			}
		}
			break;
		case NONE:
			break;
	}

	animator_.updateSubRect(*this, frametime);
	weapon_.onUpdate(frametime);
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

