#include <SFML/System.hpp>

#include "ParticleSystem.hpp"
#include "Constants.hpp"
#include "SoundSystem.hpp"
#include "utils/Math.hpp"
#include "Resources.hpp"



ParticleSystem& ParticleSystem::GetInstance()
{
	static ParticleSystem self;
	return self;
}


ParticleSystem::ParticleSystem()
{
}


ParticleSystem::~ParticleSystem()
{
	Clear();
}


void ParticleSystem::ImpactSfx(const sf::Vector2f& pos, int count)
{
	static const sf::Texture& img = Resources::getTexture("particles/impact.png");
	for (; count > 0; --count)
		particles_.push_front(new Fiery(pos, img));
}


void ParticleSystem::GreenImpactSfx(const sf::Vector2f& pos, int count)
{
	static const sf::Texture& img = Resources::getTexture("particles/impact-green.png");
	for (;count > 0; --count)
		particles_.push_front(new Fiery(pos, img));
	SoundSystem::GetInstance().PlaySound("boom.ogg");
}


void ParticleSystem::FierySfx(const sf::Vector2f& pos, int count)
{
	static const sf::Texture& fiery = Resources::getTexture("particles/fiery.png");
	for (; count > 0; --count)
		particles_.push_front(new Fiery(pos, fiery));
}


void ParticleSystem::SnowflakeSfx(const sf::Vector2f& pos, int count)
{
	static const sf::Texture& snowflake = Resources::getTexture("particles/snowflake.png");
	for (; count > 0; --count)
		particles_.push_front(new Fiery(pos, snowflake));
}


void ParticleSystem::AddStars(int count)
{
	const sf::Texture& img = Resources::getTexture("particles/star.png");
	for (; count > 0; --count)
	{
		particles_.push_front(new Star(img));
	}
}


void ParticleSystem::AddCenteredStars(int count)
{
	const sf::Texture& img = Resources::getTexture("particles/star.png");
	for (; count > 0; --count)
	{
		particles_.push_front(new CenteredStar(img));
	}
}


void ParticleSystem::AddShield(int count, const sf::Sprite* handle)
{
	float angle = 2 * math::PI / count;
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new ShieldParticle(handle, angle * (i + 1)));
	}
}


void ParticleSystem::AddSmoke(int count, const sf::Sprite* handle)
{
	const sf::Texture& img = Resources::getTexture("particles/smoke.png");
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new Smoke(img, handle));
	}
}


void ParticleSystem::RemoveShield(const sf::Sprite* handle)
{
	ParticleList::iterator it;
	for (it = particles_.begin(); it != particles_.end();)
	{
		ShieldParticle* p = dynamic_cast<ShieldParticle*>(*it);
		if (p != NULL && p->GetHandle() == handle)
		{
			delete *it;
			it = particles_.erase(it);
		}
		else
		{
			++it;
		}
	}
}


void ParticleSystem::ClearSmoke(const sf::Sprite* handle)
{
	for (ParticleList::iterator it = particles_.begin();
		it != particles_.end(); ++it)
	{
		Smoke* p = dynamic_cast<Smoke*>(*it);
		if (p != NULL && p->GetHandle() == handle)
		{
			p->SetHandle(NULL);
		}
	}
}


void ParticleSystem::Update(float frametime)
{
	ParticleList::iterator it;
	for (it = particles_.begin(); it != particles_.end();)
	{
		if ((*it)->OnUpdate(frametime))
		{
			delete *it;
			it = particles_.erase(it);
		}
		else
		{
			++it;
		}
	}
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	ParticleList::const_iterator it = particles_.begin();
	for (; it != particles_.end(); ++it)
	{
		target.draw(**it, states);
	}
}


void ParticleSystem::Clear()
{
	ParticleList::iterator it;
	for (it = particles_.begin(); it != particles_.end(); ++it)
	{
		delete *it;
	}
	particles_.clear();
}

// particles -------------------------------------------------------------------

// Fiery
#define FIERY_VELOCITY          150
#define FIERY_MIN_LIFETIME      1.0f
#define FIERY_MAX_LIFETIME      4.0f

ParticleSystem::Fiery::Fiery(const sf::Vector2f& offset, const sf::Texture& img)
{
	sprite_.setTexture(img);
	sprite_.setPosition(offset);
	//sprite_.SetBlendMode(sf::Blend::Add);
	angle_ = math::random(-math::PI, math::PI);
	sprite_.setRotation(-math::to_deg(angle_));
	float scale = math::random(0.5f, 1.2f);
	sprite_.setScale(scale, scale);
	lifetime_ = math::random(FIERY_MIN_LIFETIME, FIERY_MAX_LIFETIME);
	timer_ = 0.f;
}


bool ParticleSystem::Fiery::OnUpdate(float frametime)
{
	timer_ += frametime;
	float speed = (lifetime_ - timer_) * FIERY_VELOCITY * frametime;
	sprite_.move(speed * std::cos(angle_), -speed * std::sin(angle_));
	// transparence
	sprite_.setColor(sf::Color(255, 255, 255, (int) (255 - 255 * timer_ / lifetime_)));
	return timer_ >= lifetime_;
}


// Star
#define STAR_MIN_SPEED       10.0f
#define STAR_MAX_SPEED       300.0f

ParticleSystem::Star::Star(const sf::Texture& img)
{
	sprite_.setTexture(img);
	float x = math::random(0, APP_WIDTH);
	float y = math::random(0, APP_HEIGHT);
	sprite_.setPosition(x, y);
	float scale = math::random(1.f, 2.f);
	sprite_.setScale(scale, scale);
	sprite_.setColor(math::random_color(200, 200, 200, 255, 255, 255));
	speed_ = (int) (math::random(STAR_MIN_SPEED, STAR_MAX_SPEED));
}


bool ParticleSystem::Star::OnUpdate(float frametime)
{
	if (sprite_.getPosition().x < 0)
	{
		sprite_.setX(APP_WIDTH);
		sprite_.setY(math::random(0, APP_HEIGHT));
		speed_ = math::random(STAR_MIN_SPEED, STAR_MAX_SPEED);
		float scale = math::random(0.5f, 1.5f);
		sprite_.setScale(scale, scale);
	}
	sprite_.move(-speed_ * frametime, 0);
	return false;
}


ParticleSystem::CenteredStar::CenteredStar(const sf::Texture& img):
	ParticleSystem::Star(img)
{
	float x = APP_WIDTH / 2;
	float y = APP_HEIGHT / 2;
	sprite_.setPosition(x, y);
	float scale = math::random(0.5f, 1.5f);
	sprite_.setScale(scale, scale);
	speed_ = (int) (math::random(STAR_MIN_SPEED, STAR_MAX_SPEED));
	angle_ = math::random(-math::PI, math::PI);
}


bool ParticleSystem::CenteredStar::OnUpdate(float frametime)
{
	static const sf::FloatRect UNIVERSE(0, 0, APP_WIDTH, APP_HEIGHT);
	sf::Vector2f pos = sprite_.getPosition();
	if (!UNIVERSE.contains(pos.x, pos.y))
	{
		pos.x = APP_WIDTH / 2;
		pos.y = APP_HEIGHT / 2;
		speed_ = math::random(STAR_MIN_SPEED, STAR_MAX_SPEED);
		float scale = math::random(0.5f, 1.5f);
		sprite_.setScale(scale, scale);
		angle_ = math::random(-math::PI, math::PI);
	}

	math::translate(pos, angle_, speed_ * frametime);
	sprite_.setPosition(pos);
	return false;
}


// distance vaisseau <-> particule (plus elle est éloignée, plus elle va vite)
#define SHIELD_RADIUS 42

ParticleSystem::ShieldParticle::ShieldParticle(const sf::Sprite* handle,
	float angle)
{
	sprite_.setTexture(Resources::getTexture("particles/shield.png"));
	sprite_.setOrigin(sprite_.getCenter());
	handle_ = handle;

	angle_ = angle;
	sprite_.setRotation(-math::to_deg(angle + 0.5 * math::PI));
}


bool ParticleSystem::ShieldParticle::OnUpdate(float frametime)
{
	sf::Vector2f offset = handle_->getPosition();
	angle_ += (2 * math::PI * frametime); // rotation de 2 * PI par seconde
	offset.x += handle_->getTextureRect().width / 2;
	offset.y += handle_->getTextureRect().height / 2;
	offset.x = offset.x + (SHIELD_RADIUS) * std::cos(angle_) + frametime;
	offset.y = offset.y - (SHIELD_RADIUS) * std::sin(angle_) + frametime;
	sprite_.setPosition(offset);
	sprite_.setRotation(-math::to_deg(angle_ + (0.5 * math::PI)));
	return false;
}


// SmokeParticle
#define SMOKE_MAX_LIFETIME 2.5f
#define SMOKE_MIN_SPEED  50
#define SMOKE_MAX_SPEED  100
#define SMOKE_MIN_SIZE     0.5f
#define SMOKE_MAX_SIZE     1.2f
#define SMOKE_MIN_ANGLE    (math::PI - 0.5f)
#define SMOKE_MAX_ANGLE    (math::PI + 0.5f)


ParticleSystem::Smoke::Smoke(const sf::Texture& img, const sf::Sprite* handle)
{
	handle_ = handle;
	const sf::Vector2f& pos = handle->getPosition();
	sprite_.setPosition(pos.x, pos.y);
	sprite_.setTexture(img);
	float size = math::random(SMOKE_MIN_SIZE, SMOKE_MAX_SIZE);
	sprite_.setScale(size, size);
	float angle = math::random(SMOKE_MIN_ANGLE, SMOKE_MAX_ANGLE);
	float base_speed = math::random(SMOKE_MIN_SPEED, SMOKE_MAX_SPEED);
	vspeed_.x = std::cos(angle) * base_speed;
	vspeed_.y = -std::sin(angle) * base_speed;
	y_offset_ = (handle_->getTextureRect().height - sprite_.getHeight()) / 2;

	timer_ = math::random(0.f, SMOKE_MAX_LIFETIME);
}


bool ParticleSystem::Smoke::OnUpdate(float frametime)
{
	timer_ += frametime;
	float size = sprite_.getScale().x;
	size += frametime * 0.5;
	sprite_.setScale(size, size);
	if (timer_ >= SMOKE_MAX_LIFETIME)
	{
		if (handle_ == NULL)
		{
			return true;
		}
		timer_ = math::random(0.f, SMOKE_MAX_LIFETIME);
		float size = math::random(SMOKE_MIN_SIZE, SMOKE_MAX_SIZE);
		sprite_.setScale(size, size);
		sprite_.setPosition(handle_->getPosition().x, handle_->getPosition().y + y_offset_);
	}


	//sf::Vector2f pos = sprite_.getPosition();
	//int speed = (int) ((SMOKE_MAX_LIFETIME - timer_) * SMOKE_BASE_SPEED * frametime);

	sprite_.move(vspeed_.x * frametime, vspeed_.y * frametime);
	//sprite_.setPosition(pos);
	sprite_.setColor(sf::Color(255, 255, 255, (sf::Uint8) (255 - 255 * timer_ / SMOKE_MAX_LIFETIME)));

	return false;
}


/*
	// MAGNET STUFF
	sf::Vector2f offset = handle_->getPosition();
	sf::Vector2f myp = sprite_.getPosition();
	offset.x = offset.x + (handle_->GetSize().x / 2) + sf::Randomizer::Random(-100, 100);
	offset.y = offset.y + (handle_->GetSize().y / 2) + sf::Randomizer::Random(-100, 100);
	angle_ = math::angle(offset, myp);
	float d = math::distance(myp, offset) * 2;
	float dist =  d * frametime;
	math::translate(myp, angle_, dist);
	sprite_.setPosition(myp);
*/
