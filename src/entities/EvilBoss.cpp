#include "EvilBoss.hpp"

#include "core/ParticleSystem.hpp"
#include "core/SoundSystem.hpp"
#include "utils/Resources.hpp"

#define EYE_OFFSET_LEFT   sf::Vector2f(105, 55)
#define EYE_OFFSET_RIGHT  sf::Vector2f(190, 55)
#define MOUTH_OFFSET      sf::Vector2f(143, 128)

#define MAX_X 350
#define MAX_Y (EntityManager::GetInstance().GetHeight() - GetSize().y)

EvilBoss::EvilBoss(const sf::Vector2f& position) :
	Entity(position, EVIL)
{
	SetImage(Resources::GetImage("entities/evil-boss.png"));
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
	phase_ = EVIL;
	next_ = MORE_EVIL;
	speed_x_ = -100;
	speed_y_ = 70;
}


EvilBoss* EvilBoss::Clone() const
{
	return new EvilBoss(*this);
}


void EvilBoss::Update(float frametime)
{
	sf::Vector2f target_pos = target_->GetCenter_();

	eye_left_.ShootAt(target_pos);
	eye_right_.ShootAt(target_pos);

	if (canon_.IsInited())
	{
		canon_.ShootAt(target_pos);
	}

	const sf::Vector2f& pos = GetPosition();
	if ((int) pos.y < 60 || (int) pos.y > MAX_Y - 60)
	{
		if      ((int) pos.y < 60)         SetY(60);
		else if ((int) pos.y > MAX_Y - 60) SetY(MAX_Y - 60);
		speed_y_ *= -1;
	}
	sf::Sprite::Move(0, speed_y_ * frametime);

	bool flipped = pos.x < target_->GetPosition().x;
	if (flipped != IsFlippedX())
	{
		FlipX(flipped);
	}

	if (pos.x > MAX_X)
	{
		sf::Sprite::Move(speed_x_ * frametime, 0);
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
}


void EvilBoss::OnDestroy()
{
	sf::Vector2f pos = GetCenter_();
	ParticleSystem::GetInstance().AddGreenImpact(pos, 200);
	SoundSystem::GetInstance().PlaySound(Resources::GetSoundBuffer("boom.ogg"));
}


void EvilBoss::SetTarget(Entity* target)
{
	target_ = target;
}

