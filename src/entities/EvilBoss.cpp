#include "EvilBoss.hpp"

#include "core/ParticleSystem.hpp"
#include "core/SoundSystem.hpp"
#include "utils/MediaManager.hpp"


#define SPEED_Y 25
#define SPEED_X 100

#define EYE_OFFSET_LEFT   sf::Vector2f(105, 55)
#define EYE_OFFSET_RIGHT  sf::Vector2f(190, 55)
#define MOUTH_OFFSET      sf::Vector2f(143, 128)

#define POS_X 360

EvilBoss::EvilBoss(const sf::Vector2f& position) :
	Entity(position, EVIL)
{
	SetImage(GET_IMG("entities/evil-boss"));
	SetSubRect(sf::IntRect(0, 0, 242, 160));
	SetTeam(Entity::BAD);

	// init weapons
	eye_left_.Init("devil-eyes");
	eye_left_.SetOwner(this);
	eye_left_.SetOffset(EYE_OFFSET_LEFT);
	eye_right_.Init("devil-eyes");
	eye_right_.SetOwner(this);
	eye_right_.SetOffset(EYE_OFFSET_RIGHT);

	canon_.SetOwner(this); // (init canon later)
	canon_.SetOffset(MOUTH_OFFSET);

	target_ = NULL;
	left_ = true;
	phase_ = EVIL;
	next_ = MORE_EVIL;
}


EvilBoss* EvilBoss::Clone() const
{
	return new EvilBoss(*this);
}


void EvilBoss::Update(float frametime)
{
	sf::Vector2f target_pos = target_->GetPosition();
	target_pos.x += target_->GetSize().x / 2;
	target_pos.y += target_->GetSize().y / 2;

	eye_left_.ShootAt(target_pos);
	eye_right_.ShootAt(target_pos);

	if (canon_.IsInited())
	{
		canon_.ShootAt(target_pos);
	}

	bool left = true;
	static bool direction_ = false;
	static float mover_ = 10.0;
	float moving = SPEED_Y * frametime;
	mover_ -= moving;
	if (mover_ < 1 && mover_ > -1)
	{
		mover_ = 30.0;
		direction_ ^= 1;
	}
	if (direction_)
	{
		sf::Sprite::Move(0, -moving);
	}
	else
	{
		sf::Sprite::Move(0, moving);
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
	if (GetPosition().x > POS_X)
	{
		sf::Sprite::Move(-SPEED_X * frametime, 0);
	}
	eye_left_.Update(frametime);
	eye_right_.Update(frametime);
	canon_.Update(frametime);
}


void EvilBoss::TakeDamage(int damage)
{
	Entity::TakeDamage(damage);
	if (GetHP() < next_ && GetHP() > 0)
	{
		phase_ = next_;
		switch (phase_)
		{
			case MORE_EVIL:
				SetSubRect(sf::IntRect(242, 0, 242 * 2, 160));
				canon_.Init("laser-red");
				next_ = DAMN_EVIL;
				break;
			case DAMN_EVIL:
				SetSubRect(sf::IntRect(242 * 2, 0, 242 * 3, 160));
				canon_.SetMultiply(3);
				eye_left_.SetMultiply(3);
				eye_right_.SetMultiply(3);
				break;
			default:
				break;
		}
	}
	else if (IsDead())
	{
		sf::Vector2f pos = GetPosition();
		pos.x += GetSize().x / 2;
		pos.y += GetSize().y / 2;
		ParticleSystem::GetInstance().AddGreenImpact(pos, 200);
		SoundSystem::GetInstance().PlaySound("boom");
	}
}


bool EvilBoss::PixelPerfectCollide() const
{
	return true;
}


void EvilBoss::SetTarget(Entity* target)
{
	target_ = target;
}

