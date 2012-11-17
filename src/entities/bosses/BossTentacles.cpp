#include "BossTentacles.hpp"
#include "entities/EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "utils/Math.hpp"

#define MAX_X 400
#define MAX_Y (EntityManager::GetInstance().GetHeight() - getHeight())


BossTentacles::BossTentacles(const sf::Vector2f& position) :
	Entity(position, 300)
{
	SetTeam(Entity::BAD);

	// init weapons
	weapon_.Init("laser-pink");
	weapon_.SetOwner(this);
	weapon_.SetOffset(74, 42);
	weapon_.SetMultiply(3);


	target_ = NULL;
	move_ = INIT;
	speed_x_ = -100;
	speed_y_ = 0;
	timer_ = 0;

	m_animator.setAnimation(*this, EntityManager::GetInstance().GetAnimation("boss-tentacles"));
}


BossTentacles* BossTentacles::Clone() const
{
	return new BossTentacles(*this);
}


void BossTentacles::Update(float frametime)
{
	m_animator.updateSubRect(*this, frametime);

	timer_ += frametime;

	sf::Vector2f target_pos = target_->getPosition();
	target_pos.x += target_->getWidth() / 2;
	target_pos.y += target_->getHeight() / 2;

	weapon_.ShootAt(target_pos);


	sf::Vector2f pos = getPosition();
	switch (move_)
	{
		case INIT:
			if (pos.x < MAX_X)
			{
				setX(MAX_X);
				move_ = LURK;
				speed_y_ = 120;
				speed_x_ = 0;
			}
			break;
		case LURK:
			if ((int) pos.y < 40 || (int) pos.y > MAX_Y - 40)
			{
				if      ((int) pos.y < 40)         setY(40);
				else if ((int) pos.y > MAX_Y - 40) setY(MAX_Y - 40);
				speed_y_ *= -1;
			}
			if ((int) timer_ > 10)
			{
				timer_ = 0.f;
				move_ = IDLE;
				speed_x_ = speed_y_ = 0;
			}
			break;
		case IDLE:
			if ((int) timer_ > 2)
			{
				timer_ = 0.f;
				move_ = CHARGE;
				float angle = math::angle(target_pos, pos);
				speed_x_ = std::cos(angle) * 350;
				speed_y_ = -std::sin(angle) * 350;
			}
			break;
		case CHARGE:
			if ((int) pos.x < 10 || (int) pos.y < 10 || (int) pos.y > MAX_Y - 10)
			{
				if ((int) pos.x < 10)         setX(10);
				if ((int) pos.y < 10)         setY(10);
				if ((int) pos.y > MAX_Y - 10) setY(MAX_Y - 10);
				speed_x_ *= -1;
				speed_y_ *= -1;
			}
			else if ((int) pos.x > MAX_X)
			{
				setX(MAX_X);
				move_ = LURK;
				speed_x_ = 0;
				speed_y_ = 120;
			}
			break;
	}
	sf::Sprite::move(speed_x_ * frametime, speed_y_ * frametime);
	weapon_.Update(frametime);
	Entity::UpdateFlash(frametime);
}


void BossTentacles::OnDestroy()
{
	sf::Vector2f pos = getCenter();
	ParticleSystem::GetInstance().GreenImpactSfx(pos, 100);
}


void BossTentacles::SetTarget(Entity* target)
{
	target_ = target;
}

