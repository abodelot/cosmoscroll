#include "Boss.hpp"
#include "EntityManager.hpp"
#include "core/ParticleSystem.hpp"
#include "core/Resources.hpp"

#define EYE_OFFSET_LEFT   sf::Vector2f(105, 55)
#define EYE_OFFSET_LEFT_SMALL  sf::Vector2f(52.5, 27.5)
#define EYE_OFFSET_RIGHT  sf::Vector2f(190, 55)
#define EYE_OFFSET_RIGHT_SMALL  sf::Vector2f(95, 27.5)
#define MOUTH_OFFSET      sf::Vector2f(143, 128)
#define MOUTH_OFFSET_SMALL     sf::Vector2f(72.5, 64)


#define MAX_X (EntityManager::GetInstance().GetWidth() - getWidth())
#define MAX_Y (EntityManager::GetInstance().GetHeight() - getHeight())

Boss::Boss(sf::Vector2f& pos , int HP, bool split) : Entity(pos,HP)
{
    if(split)
    {
        setTexture(Resources::getTexture("entities/evil-boss.png"));
        setTextureRect(sf::IntRect(0, 0, 242, 160));

        eye_left_.Init("devil-eyes");
        eye_left_.SetOwner(this);
        eye_left_.SetOffset(EYE_OFFSET_LEFT);
        eye_right_.Init("devil-eyes");
        eye_right_.SetOwner(this);
        eye_right_.SetOffset(EYE_OFFSET_RIGHT);
        // hack: disable sound on the second eye so it won't be played twice
        eye_right_.SetSound(NULL);

        canon_.Init("laser-pink");
        canon_.SetOwner(this); // (init canon later)
        canon_.SetOffset(MOUTH_OFFSET);
    }
    else
    {
        setTexture(Resources::getTexture("entities/evil-boss-small.png"));
        setTextureRect(sf::IntRect(0,0,121,80));

        eye_left_.Init("devil-eyes");
        eye_left_.SetOwner(this);
        eye_left_.SetOffset(EYE_OFFSET_LEFT_SMALL);
        eye_right_.Init("devil-eyes");
        eye_right_.SetOwner(this);
        eye_right_.SetOffset(EYE_OFFSET_RIGHT_SMALL);
        // hack: disable sound on the second eye so it won't be played twice
        eye_right_.SetSound(NULL);

        canon_.Init("laser-pink");
        canon_.SetOwner(this); // (init canon later)
        canon_.SetOffset(MOUTH_OFFSET_SMALL);
    }

	SetTeam(Entity::BAD);
    target_ = NULL;
	speed_x_ = -70;
	speed_y_ = -70;
	split_mode_ = split;
}

void Boss::Update(float frametime)
{
    //Attack
	sf::Vector2f target_pos = target_->getCenter();

	eye_left_.ShootAt(target_pos);
	eye_right_.ShootAt(target_pos);
	canon_.ShootAt(target_pos);

    //Movement

    //Bound Check
    const sf::Vector2f& pos = getPosition();

    if( (int)pos.x <= 0)
        {
            setX(0);
            speed_x_ *= -1;
        }
        else if((int)pos.x >= MAX_X)
        {
            //To Allow Rebounds and [**Slide in From Right @ Entry** ]
            if(speed_x_ > 0)
                {
                    setX(MAX_X);
                    speed_x_ *= -1;
                }
        }

        if((int) pos.y < 0)
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

	eye_left_.Update(frametime);
	eye_right_.Update(frametime);
	canon_.Update(frametime);
	Entity::UpdateFlash(frametime);
}

void Boss::TakeDamage(int damage)
{
    Entity::TakeDamage(damage);
    if(GetHP() == 0 && split_mode_)
    {
        sf::Vector2f pos = getPosition();
        Boss *small_boss2 = new Boss(pos,150,false);
        EntityManager::GetInstance().AddEntity(small_boss2);

         sf::Vector2f pos1 = sf::Vector2f(pos.x ,pos.y - 10);
        Boss *small_boss1 = new Boss(pos1,150,false);
        small_boss1->invert_speed_y();
        EntityManager::GetInstance().AddEntity(small_boss1);
    }
}

void Boss::SetTarget(Entity* target)
{
    target_ = target;
}

void Boss::OnDestroy()
{
    ParticleSystem::GetInstance().GreenImpactSfx(getCenter(), 200);
}

Boss* Boss::Clone() const
{
    return new Boss(*this);
}


void Boss::invert_speed_y()
{
    speed_y_ *= -1;
}
