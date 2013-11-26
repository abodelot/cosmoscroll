#include "EvilBoss.hpp"
#include "entities/Player.hpp"
#include "core/ParticleSystem.hpp"
#include "core/Resources.hpp"

#define EYE_OFFSET_LEFT   sf::Vector2f(105, 55)
#define EYE_OFFSET_RIGHT  sf::Vector2f(190, 55)
#define MOUTH_OFFSET      sf::Vector2f(143, 128)

#define MAX_X 350
#define MAX_Y (EntityManager::getInstance().getHeight() - getHeight())

EvilBoss::EvilBoss()
{
	setTexture(Resources::getTexture("entities/evil-boss.png"));
	setTextureRect(sf::IntRect(0, 0, 242, 160));
	setTeam(Entity::BAD);
	setHP(EVIL);

	// init weapons
	m_eye_left.init("fireball");
	m_eye_left.setOwner(this);
	m_eye_left.setPosition(EYE_OFFSET_LEFT);
	m_eye_right.init("fireball");
	m_eye_right.setOwner(this);
	m_eye_right.setPosition(EYE_OFFSET_RIGHT);
	// hack: disable sound on the second eye so it won't be played twice
	m_eye_right.setSound(NULL);

	m_mouth.setOwner(this); // (this one must inited later)
	m_mouth.setPosition(MOUTH_OFFSET);

	target_ = NULL;
	phase_ = EVIL;
	next_ = MORE_EVIL;
	speed_x_ = -100;
	speed_y_ = 70;
}


void EvilBoss::onUpdate(float frametime)
{
	sf::Vector2f target_pos = target_->getCenter();

	m_eye_left.shoot(target_pos);
	m_eye_right.shoot(target_pos);

	if (m_mouth.isInited())
	{
		m_mouth.shoot(target_pos);
	}

	const sf::Vector2f& pos = getPosition();
	if ((int) pos.y < 60 || (int) pos.y > MAX_Y - 60)
	{
		if      ((int) pos.y < 60)         setY(60);
		else if ((int) pos.y > MAX_Y - 60) setY(MAX_Y - 60);
		speed_y_ *= -1;
	}
	sf::Sprite::move(0, speed_y_ * frametime);

	if (pos.x > MAX_X)
	{
		sf::Sprite::move(speed_x_ * frametime, 0);
	}
	m_eye_left.onUpdate(frametime);
	m_eye_right.onUpdate(frametime);
	m_mouth.onUpdate(frametime);

	updateDamageFlash(frametime);
}


void EvilBoss::takeDamage(int damage)
{
	Damageable::takeDamage(damage);
	if (getHP() < next_ && getHP() > 0 && phase_ != next_)
	{
		phase_ = next_;
		switch (phase_)
		{
			case MORE_EVIL:
				setTextureRect(sf::IntRect(242, 0, 242, 160));
				m_mouth.init("laser-pink");
				next_ = DAMN_EVIL;
				break;
			case DAMN_EVIL:
				setTextureRect(sf::IntRect(242 * 2, 0, 242, 160));
				m_mouth.setMultiply(3);
				m_eye_left.setMultiply(3);
				m_eye_right.setMultiply(3);
				break;
			default:
				break;
		}
	}
}


void EvilBoss::onDestroy()
{
	ParticleSystem::GetInstance().GreenImpactSfx(getCenter(), 200);
}


void EvilBoss::onInit()
{
    target_ = EntityManager::getInstance().GetPlayerShip();
}

