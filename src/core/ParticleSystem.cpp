#include "ParticleSystem.hpp"
#include "Game.hpp"
#include "SoundSystem.hpp"
#include "../utils/Math.hpp"

#include <SFML/System.hpp>


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


void ParticleSystem::AddExplosion(const sf::Vector2f& offset)
{
	particles_.push_front(new Explosion(offset));
	SoundSystem::GetInstance().PlaySound("boom");
}


void ParticleSystem::AddFiery(int x, int y)
{
	static const sf::Image& fiery = GET_IMG("particles/fiery");
	sf::Vector2f pos(x, y);
	for (int i = 0; i < 42; ++i)
	{
		particles_.push_front(new Fiery(pos, fiery));
	}
}


void ParticleSystem::AddImpact(const sf::Vector2f& offset, int count)
{
	static const sf::Image& img = GET_IMG("particles/impact");
	for (; count > 0; --count)
	{
		particles_.push_front(new Fiery(offset, img));
	}
}


void ParticleSystem::AddGreenImpact(const sf::Vector2f& pos, int count)
{
	static const sf::Image& img = GET_IMG("particles/impact-green");
	for (;count > 0; --count)
		particles_.push_front(new Fiery(pos, img));
}


void ParticleSystem::AddStars(int count)
{
	for (; count > 0; --count)
	{
		particles_.push_front(new Star());
	}
}


void ParticleSystem::AddCenteredStars(int count)
{
	for (; count > 0; --count)
	{
		particles_.push_front(new CenteredStar());
	}
}


void ParticleSystem::AddMessage(const sf::Vector2f& offset, const wchar_t* text)
{
	particles_.push_front(new TextParticle(offset, text));
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
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new Smoke(handle));
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
		if (p != NULL and p->GetHandle() == handle)
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
	sprite_.Move(speed * math::cos(angle_), -speed * math::sin(angle_));
	// transparence
	sprite_.SetColor(sf::Color(255, 255, 255, (int) (255 - 255 * timer_ / lifetime_)));
	return timer_ >= lifetime_;
}


// Star
#define STAR_MIN_SPEED       30.0f
#define STAR_MAX_SPEED       1000.0f

ParticleSystem::Star::Star()
{
	sprite_.SetImage(GET_IMG("particles/star"));
	float x = sf::Randomizer::Random(0, Game::WIDTH);
	float y = sf::Randomizer::Random(0, Game::HEIGHT);
	sprite_.SetPosition(x, y);
	float scale = sf::Randomizer::Random(0.5f, 1.5f);
	sprite_.SetScale(scale, scale);
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


ParticleSystem::CenteredStar::CenteredStar()
{
	sprite_.SetImage(GET_IMG("particles/star"));
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

ParticleSystem::TextParticle::TextParticle(const sf::Vector2f& offset, const wchar_t* text)
{
	text_.SetText(text);
	text_.SetSize(12);
	text_.SetColor(sf::Color::White);
	text_.SetPosition(offset);
	timer_ = 0.f;
}


bool ParticleSystem::TextParticle::OnUpdate(float frametime)
{
	timer_ += frametime;
	text_.Move(0, -MESSAGE_SPEED * frametime);
	// transparence
	text_.SetColor(sf::Color(255, 255, 255, static_cast<sf::Uint8> (255 - 255 * timer_ / MESSAGE_LIFETIME)));
	return timer_ >= MESSAGE_LIFETIME;
}



// distance vaisseau <-> particule (plus elle est éloignée, plus elle va vite)
#define SHIELD_RADIUS 42

ParticleSystem::ShieldParticle::ShieldParticle(const sf::Sprite* handle,
	float angle)
{
	sprite_.SetImage(GET_IMG("particles/shield"));
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
	offset.x = offset.x + (SHIELD_RADIUS) * math::cos(angle_) + frametime;
	offset.y = offset.y - (SHIELD_RADIUS) * math::sin(angle_) + frametime;
	sprite_.SetPosition(offset);
	sprite_.SetRotation(math::rad_to_deg(angle_ + (0.5 * PI)));
	return false;
}


// SmokeParticle
#define SMOKE_MAX_LIFETIME 4.f
#define SMOKE_BASE_SPEED   1100
#define SMOKE_MIN_SIZE     0.5f
#define SMOKE_MAX_SIZE     1.2f
#define SMOKE_MIN_ANGLE    (PI - 1.f)
#define SMOKE_MAX_ANGLE    (PI + 1.f)


ParticleSystem::Smoke::Smoke(const sf::Sprite* handle)
{
	static const sf::Image& img = GET_IMG("particles/smoke");

	handle_ = handle;
	const sf::Vector2f& pos = handle->GetPosition();
	sprite_.SetPosition(pos.x, pos.y);
	sprite_.SetImage(img);
	float size = sf::Randomizer::Random(SMOKE_MIN_SIZE, SMOKE_MAX_SIZE);
	sprite_.SetScale(size, size);
	angle_ = sf::Randomizer::Random(SMOKE_MIN_ANGLE, SMOKE_MAX_ANGLE);
	timer_ = sf::Randomizer::Random(0.0f, SMOKE_MAX_LIFETIME);
	y_offset_ = (handle_->GetSize().y - sprite_.GetSize().y) / 2;
}


bool ParticleSystem::Smoke::OnUpdate(float frametime)
{
	timer_ += frametime;
	if (timer_ >= SMOKE_MAX_LIFETIME)
	{
		if (handle_ == NULL)
		{
			return true;
		}
		timer_ = 0;
		sprite_.SetPosition(handle_->GetPosition().x, handle_->GetPosition().y + y_offset_);
		angle_ = sf::Randomizer::Random(SMOKE_MIN_ANGLE, SMOKE_MAX_ANGLE);
	}


	sf::Vector2f pos = sprite_.GetPosition();
	int speed = (int) ((SMOKE_MAX_LIFETIME - timer_) * SMOKE_BASE_SPEED * frametime);

	math::translate(pos, angle_, speed * frametime);
	sprite_.SetPosition(pos);
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
