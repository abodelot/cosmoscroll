#include "SplitBoss.hpp"
#include "entities/EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "core/Resources.hpp"

#define EYE_OFFSET_LEFT   sf::Vector2f(105, 55)
#define EYE_OFFSET_LEFT_SMALL  sf::Vector2f(52.5, 27.5)
#define EYE_OFFSET_RIGHT  sf::Vector2f(190, 55)
#define EYE_OFFSET_RIGHT_SMALL  sf::Vector2f(95, 27.5)

#define MAX_X (EntityManager::GetInstance().GetWidth() - getWidth())
#define MAX_Y (EntityManager::GetInstance().GetHeight() - getHeight())

SplitBoss::SplitBoss(sf::Vector2f& pos, int HP, bool split) : Entity(pos,HP)
{
	if (split)
	{
		setTexture(Resources::getTexture("entities/evil-boss.png"));
		setTextureRect(sf::IntRect(0, 0, 242, 160));

		eye_left_.init("devil-eyes");
		eye_left_.setOwner(this);
		eye_left_.setPosition(EYE_OFFSET_LEFT);
		eye_right_.init("devil-eyes");
		eye_right_.setOwner(this);
		eye_right_.setPosition(EYE_OFFSET_RIGHT);
	}
	else
	{
		setTexture(Resources::getTexture("entities/evil-boss-small.png"));
		setTextureRect(sf::IntRect(0,0,121,80));

		eye_left_.init("devil-eyes");
		eye_left_.setOwner(this);
		eye_left_.setPosition(EYE_OFFSET_LEFT_SMALL);
		eye_right_.init("devil-eyes");
		eye_right_.setOwner(this);
		eye_right_.setPosition(EYE_OFFSET_RIGHT_SMALL);
	}
	// hack: disable sound on the second eye so it won't be played twice
	eye_right_.setSound(NULL);
	SetTeam(Entity::BAD);
	target_ = NULL;
	speed_x_ = -70;
	speed_y_ = -70;
	split_mode_ = split;
}

void SplitBoss::Update(float frametime)
{
    //Attack
	sf::Vector2f target_pos = target_->getCenter();

	eye_left_.shoot(target_pos);
	eye_right_.shoot(target_pos);

    //Movement

	//Bound Check
    const sf::Vector2f& pos = getPosition();

    if ((int)pos.x <= 0)
	{
		setX(0);
		speed_x_ *= -1;
	}
	else if ((int)pos.x >= MAX_X)
	{
		//To Allow Rebounds and [**Slide in From Right @ Entry** ]
		if (speed_x_ > 0)
		{
			setX(MAX_X);
			speed_x_ *= -1;
		}
	}

	if ((int) pos.y < 0)
	{
		 setY(0);
		 speed_y_ *= -1;
	}
	else if ((int) pos.y >= MAX_Y )
	{
		setY(MAX_Y );
		speed_y_ *= -1;
	}

    //Normal Movement

    sf::Sprite::move(speed_x_*frametime , speed_y_*frametime);

	eye_left_.onUpdate(frametime);
	eye_right_.onUpdate(frametime);
	Entity::UpdateFlash(frametime);
}

void SplitBoss::TakeDamage(int damage)
{
    Entity::TakeDamage(damage);
    if (IsDead() && split_mode_)
    {
        sf::Vector2f pos = getPosition();
        SplitBoss *small_boss2 = new SplitBoss(pos,150,false);
        EntityManager::GetInstance().AddEntity(small_boss2);

        sf::Vector2f pos1 = sf::Vector2f(pos.x ,pos.y - 10);
        SplitBoss *small_boss1 = new SplitBoss(pos1,150,false);
        small_boss1->speed_y_ *= -1;
        EntityManager::GetInstance().AddEntity(small_boss1);
    }
}

void SplitBoss::SetTarget(Entity* target)
{
    target_ = target;
}

void SplitBoss::OnDestroy()
{
    ParticleSystem::GetInstance().GreenImpactSfx(getCenter(), 200);
}

SplitBoss* SplitBoss::Clone() const
{
    return new SplitBoss(*this);
}

