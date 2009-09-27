#include "EvilBoss.hpp"
#include "EntityManager.hpp"
#include "../core/Game.hpp"
#include "../core/ParticleSystem.hpp"
#include "../utils/MediaManager.hpp"
#include "../utils/Math.hpp"

#define SPEED 25
#define CHILDS 8

#define L_EYE_OFFSET    sf::Vector2f(105, 55)
#define R_EYE_OFFSET    sf::Vector2f(190, 55)

#define L_MOUTH_X_OFFSET 85
#define R_MOUTH_X_OFFSET 201
#define MOUTH_Y_OFFSET   128


EvilBoss::EvilBoss(const sf::Vector2f& position) :
	Entity(position, EVIL)
{
	SetImage(GET_IMG("evil_boss"));
	SetSubRect(sf::IntRect(0, 0, 242, 160));
	SetTeam(Entity::BAD);
	// init weapons
	EntityManager& mgr = EntityManager::GetInstance();
	mgr.InitWeapon(0, &eye_left_);
	mgr.InitWeapon(0, &eye_right_);
	// (init canon later)
	eye_left_.SetOwner(this);
	eye_right_.SetOwner(this);
	canon_.SetOwner(this);

	left_ = true;
	phase_ = EVIL;
	next_ = MORE_EVIL;
	target_ = Game::GetInstance().GetPlayerShip();
}


EvilBoss* EvilBoss::Clone() const
{
	return new EvilBoss(*this);
}


void EvilBoss::Update(float frametime)
{
	float radians_L = math::angle(target_->GetPosition(), GetPosition() + L_EYE_OFFSET);
	float radians_R = math::angle(target_->GetPosition(), GetPosition() + R_EYE_OFFSET);

	eye_left_.Shoot(GetPosition() + L_EYE_OFFSET, radians_L);
	eye_right_.Shoot(GetPosition() + R_EYE_OFFSET, radians_R);
	sf::Vector2f randV2f, my = GetPosition();
	randV2f.x = my.x + sf::Randomizer::Random(L_MOUTH_X_OFFSET, R_MOUTH_X_OFFSET);
	randV2f.y = my.y + MOUTH_Y_OFFSET;
	if (canon_.IsInited())
	{
		canon_.Shoot(randV2f, math::angle(target_->GetPosition(), randV2f));
	}

	bool left = true;
	static bool direction_ = false;
	static float mover_ = 10.0;
	float moving_ = SPEED * frametime;
	mover_ -= moving_;
	if (mover_ < 1 && mover_ > -1)
	{
		mover_ = 30.0;
		direction_ ^= 1;
	}
	if (direction_)
	{
		sf::Sprite::Move(0, -SPEED * frametime);
	}
	else
	{
		sf::Sprite::Move(0, SPEED * frametime);
	}

	if (GetPosition().x < target_->GetPosition().x)
	{
		left = false;
	}

	if (left != left_)
	{
		FlipX(!left);
		left_ = left;
	}
	eye_left_.Update(frametime);
	eye_right_.Update(frametime);
	canon_.Update(frametime);
}


void EvilBoss::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
	if (hp_ < next_ && hp_ > 0)
	{
		phase_ = next_;
		switch (phase_)
		{
			case MORE_EVIL:
				SetSubRect(sf::IntRect(242, 0, 242 * 2, 160));
				EntityManager::GetInstance().InitWeapon(3, &canon_);
				next_ = DAMN_EVIL;
				break;
			case DAMN_EVIL:
				SetSubRect(sf::IntRect(242 * 2, 0, 242 * 3, 160));
				canon_.SetTriple(true);
				break;
			default:
				break;
		}
	}
	else if (hp_ <= 0)
	{
		sf::Vector2f pos = GetPosition();
		pos.x += GetSize().x / 2;
		pos.y += GetSize().y / 2;
		for (int i = 0; i < 4; ++i)
		{
			ParticleSystem::GetInstance().AddExplosion(pos);
		}
	}
}
