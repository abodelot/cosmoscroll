#include <SFML/System.hpp>

#include "ParticleSystem.hpp"
#include "Game.hpp"
#include "SoundSystem.hpp"
#include "utils/Math.hpp"
#include "utils/Resources.hpp"



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

void ParticleSystem::ExplosionSfx(const sf::Vector2f& offset)
{
	particles_.push_front(new Explosion(offset));
	SoundSystem::GetInstance().PlaySound("boom.ogg");
}


void ParticleSystem::ImpactSfx(const sf::Vector2f& pos, int count)
{
	static const sf::Image& img = Resources::GetImage("particles/impact.png");
	for (; count > 0; --count)
		particles_.push_front(new Fiery(pos, img));
}


void ParticleSystem::GreenImpactSfx(const sf::Vector2f& pos, int count)
{
	static const sf::Image& img = Resources::GetImage("particles/impact-green.png");
	for (;count > 0; --count)
		particles_.push_front(new Fiery(pos, img));
	SoundSystem::GetInstance().PlaySound("boom.ogg");
}


void ParticleSystem::FierySfx(const sf::Vector2f& pos, int count)
{
	static const sf::Image& fiery = Resources::GetImage("particles/fiery.png");
	for (; count > 0; --count)
		particles_.push_front(new Fiery(pos, fiery));
}


void ParticleSystem::SnowflakeSfx(const sf::Vector2f& pos, int count)
{
	static const sf::Image& snowflake = Resources::GetImage("particles/snowflake.png");
	for (; count > 0; --count)
		particles_.push_front(new Fiery(pos, snowflake));
}


void ParticleSystem::AddStars(int count)
{
	const sf::Image& img = Resources::GetImage("particles/star.png");
	for (; count > 0; --count)
	{
		particles_.push_front(new Star(img));
	}
}


void ParticleSystem::AddCenteredStars(int count)
{
	const sf::Image& img = Resources::GetImage("particles/star.png");
	for (; count > 0; --count)
	{
		particles_.push_front(new CenteredStar(img));
	}
}


void ParticleSystem::AddMessage(const sf::Vector2f& offset, const sf::Unicode::Text& text, const sf::Color& color)
{
	particles_.push_front(new TextParticle(offset, text, color));
}


void ParticleSystem::AddShield(int count, const sf::Sprite* handle)
{
	float angle = 2 * PI / count;
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new ShieldParticle(handle, angle * (i + 1)));
	}
}


void ParticleSystem::AddSmoke(int count, const sf::Sprite* handle)
{
	const sf::Image& img = Resources::GetImage("particles/smoke.png");
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new Smoke(img, handle));
	}
}


/*template <class TParticle>
void ParticleSystem::RemoveLinkedBy(const sf::Sprite* handle)
{
}*/

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


void ParticleSystem::Show(sf::RenderTarget& target) const
{
	ParticleList::const_iterator it = particles_.begin();
	for (; it != particles_.end(); ++it)
	{
		(**it).Show(target);
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

ParticleSystem::Fiery::Fiery(const sf::Vector2f& offset, const sf::Image& img)
{
	sprite_.SetImage(img);
	sprite_.SetPosition(offset);
	sprite_.SetBlendMode(sf::Blend::Add);
	angle_ = sf::Randomizer::Random(-PI, PI);
	sprite_.SetRotation(math::rad_to_deg(angle_));
	float scale = sf::Randomizer::Random(0.5f, 1.2f);
	sprite_.SetScale(scale, scale);
	lifetime_ = sf::Randomizer::Random(FIERY_MIN_LIFETIME, FIERY_MAX_LIFETIME);
	timer_ = 0.f;
}


bool ParticleSystem::Fiery::OnUpdate(float frametime)
{
	timer_ += frametime;
	float speed = (lifetime_ - timer_) * FIERY_VELOCITY * frametime;
	sprite_.Move(speed * std::cos(angle_), -speed * std::sin(angle_));
	// transparence
	sprite_.SetColor(sf::Color(255, 255, 255, (int) (255 - 255 * timer_ / lifetime_)));
	return timer_ >= lifetime_;
}


// Star
#define STAR_MIN_SPEED       30.0f
#define STAR_MAX_SPEED       900.0f

ParticleSystem::Star::Star(const sf::Image& img)
{
	sprite_.SetImage(img);
	float x = sf::Randomizer::Random(0, Game::WIDTH);
	float y = sf::Randomizer::Random(0, Game::HEIGHT);
	sprite_.SetPosition(x, y);
	float scale = sf::Randomizer::Random(1.f, 2.f);
	sprite_.SetScale(scale, scale);
	sprite_.SetColor(math::random_color(200, 200, 200, 255, 255, 255));
	speed_ = (int) (sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED));
}


bool ParticleSystem::Star::OnUpdate(float frametime)
{
	if (sprite_.GetPosition().x < 0)
	{
		sprite_.SetX(Game::WIDTH);
		sprite_.SetY(sf::Randomizer::Random(0, Game::HEIGHT));
		sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED);
		float scale = sf::Randomizer::Random(0.5f, 1.5f);
		sprite_.SetScale(scale, scale);
	}
	sprite_.Move(-speed_ * frametime, 0);
	return false;
}


ParticleSystem::CenteredStar::CenteredStar(const sf::Image& img):
	ParticleSystem::Star(img)
{
	float x = Game::WIDTH / 2;
	float y = Game::HEIGHT / 2;
	sprite_.SetPosition(x, y);
	float scale = sf::Randomizer::Random(0.5f, 1.5f);
	sprite_.SetScale(scale, scale);
	speed_ = (int) (sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED));
	angle_ = sf::Randomizer::Random(-PI, PI);
}


bool ParticleSystem::CenteredStar::OnUpdate(float frametime)
{
	static const sf::FloatRect UNIVERSE(0, 0, Game::WIDTH, Game::HEIGHT);
	sf::Vector2f pos = sprite_.GetPosition();
	if (!UNIVERSE.Contains(pos.x, pos.y))
	{
		pos.x = Game::WIDTH / 2;
		pos.y = Game::HEIGHT / 2;
		sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED);
		float scale = sf::Randomizer::Random(0.5f, 1.5f);
		sprite_.SetScale(scale, scale);
		angle_ = sf::Randomizer::Random(-PI, PI);
	}

	math::translate(pos, angle_, speed_ * frametime);
	sprite_.SetPosition(pos);
	return false;
}

// TextParticle
#define MESSAGE_LIFETIME   5.0
#define MESSAGE_SPEED      50

ParticleSystem::TextParticle::TextParticle(const sf::Vector2f& offset, const sf::Unicode::Text& text, const sf::Color& color)
{
	text_.SetText(text);
	text_.SetSize(12);
	text_.SetColor(color);
	text_.SetPosition(offset);
	timer_ = 0.f;
}


bool ParticleSystem::TextParticle::OnUpdate(float frametime)
{
	timer_ += frametime;
	text_.Move(0, -MESSAGE_SPEED * frametime);
	// transparence
	sf::Color color = text_.GetColor();
	color.a = static_cast<sf::Uint8> (255 - 255 * timer_ / MESSAGE_LIFETIME);
	text_.SetColor(color);
	return timer_ >= MESSAGE_LIFETIME;
}



// distance vaisseau <-> particule (plus elle est éloignée, plus elle va vite)
#define SHIELD_RADIUS 42

ParticleSystem::ShieldParticle::ShieldParticle(const sf::Sprite* handle,
	float angle)
{
	sprite_.SetImage(Resources::GetImage("particles/shield.png"));
	sprite_.SetCenter(sprite_.GetSize().x / 2, sprite_.GetSize().y / 2);
	handle_ = handle;

	angle_ = angle;
	sprite_.SetRotation(math::rad_to_deg(angle + 0.5 * PI));
}


bool ParticleSystem::ShieldParticle::OnUpdate(float frametime)
{
	sf::Vector2f offset = handle_->GetPosition();
	angle_ += (2 * PI * frametime); // rotation de 2 * PI par seconde
	offset.x += handle_->GetSize().x / 2;
	offset.y += handle_->GetSize().y / 2;
	offset.x = offset.x + (SHIELD_RADIUS) * std::cos(angle_) + frametime;
	offset.y = offset.y - (SHIELD_RADIUS) * std::sin(angle_) + frametime;
	sprite_.SetPosition(offset);
	sprite_.SetRotation(math::rad_to_deg(angle_ + (0.5 * PI)));
	return false;
}


// SmokeParticle
#define SMOKE_MAX_LIFETIME 2.5f
#define SMOKE_MIN_SPEED  50
#define SMOKE_MAX_SPEED  100
#define SMOKE_MIN_SIZE     0.5f
#define SMOKE_MAX_SIZE     1.2f
#define SMOKE_MIN_ANGLE    (PI - 0.5f)
#define SMOKE_MAX_ANGLE    (PI + 0.5f)


ParticleSystem::Smoke::Smoke(const sf::Image& img, const sf::Sprite* handle)
{
	handle_ = handle;
	const sf::Vector2f& pos = handle->GetPosition();
	sprite_.SetPosition(pos.x, pos.y);
	sprite_.SetImage(img);
	float size = sf::Randomizer::Random(SMOKE_MIN_SIZE, SMOKE_MAX_SIZE);
	sprite_.SetScale(size, size);
	float angle = sf::Randomizer::Random(SMOKE_MIN_ANGLE, SMOKE_MAX_ANGLE);
	float base_speed = sf::Randomizer::Random(SMOKE_MIN_SPEED, SMOKE_MAX_SPEED);
	vspeed_.x = std::cos(angle) * base_speed;
	vspeed_.y = -std::sin(angle) * base_speed;
	y_offset_ = (handle_->GetSize().y - sprite_.GetSize().y) / 2;

	timer_ = sf::Randomizer::Random(0.f, SMOKE_MAX_LIFETIME);
}


bool ParticleSystem::Smoke::OnUpdate(float frametime)
{
	timer_ += frametime;
	float size = sprite_.GetScale().x;
	size += frametime * 0.5;
	sprite_.SetScale(size, size);
	if (timer_ >= SMOKE_MAX_LIFETIME)
	{
		if (handle_ == NULL)
		{
			return true;
		}
		timer_ = sf::Randomizer::Random(0.f, SMOKE_MAX_LIFETIME);
		float size = sf::Randomizer::Random(SMOKE_MIN_SIZE, SMOKE_MAX_SIZE);
		sprite_.SetScale(size, size);
		sprite_.SetPosition(handle_->GetPosition().x, handle_->GetPosition().y + y_offset_);
	}


	//sf::Vector2f pos = sprite_.GetPosition();
	//int speed = (int) ((SMOKE_MAX_LIFETIME - timer_) * SMOKE_BASE_SPEED * frametime);

	sprite_.Move(vspeed_.x * frametime, vspeed_.y * frametime);
	//sprite_.SetPosition(pos);
	sprite_.SetColor(sf::Color(255, 255, 255, (sf::Uint8) (255 - 255 * timer_ / SMOKE_MAX_LIFETIME)));

	return false;
}


/*
	// MAGNET STUFF
	sf::Vector2f offset = handle_->GetPosition();
	sf::Vector2f myp = sprite_.GetPosition();
	offset.x = offset.x + (handle_->GetSize().x / 2) + sf::Randomizer::Random(-100, 100);
	offset.y = offset.y + (handle_->GetSize().y / 2) + sf::Randomizer::Random(-100, 100);
	angle_ = math::angle(offset, myp);
	float d = math::distance(myp, offset) * 2;
	float dist =  d * frametime;
	math::translate(myp, angle_, dist);
	sprite_.SetPosition(myp);
*/
