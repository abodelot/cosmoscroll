#include "ParticleSystem.hpp"
#include "SoundSystem.hpp"
#include "Window.hpp"
#include "../utils/Math.hpp"

#include <SFML/System.hpp>

#define PARTICLES_PER_EXPLOSION 40


ParticleSystem& ParticleSystem::GetInstance()
{
	static ParticleSystem self;
	return self;
}


ParticleSystem::ParticleSystem() :
	media_(MediaManager::GetInstance())
{
}


ParticleSystem::~ParticleSystem()
{
	Clear();
}


void ParticleSystem::AddExplosion(const sf::Vector2f& offset)
{
	for (int i = 0; i < PARTICLES_PER_EXPLOSION; ++i)
	{
		particles_.push_front(new Fiery(offset, media_.GetImage("fiery")));
	}
	SoundSystem::GetInstance().PlaySound("boom");
}


void ParticleSystem::AddImpact(const sf::Vector2f& offset, int count)
{
	for (; count > 0; --count)
	{
		particles_.push_front(new Fiery(offset, media_.GetImage("impact")));
	}
}


void ParticleSystem::AddStars(int count)
{
	for (; count > 0; --count)
	{
		particles_.push_front(new Star());
	}
}


void ParticleSystem::AddMessage(const sf::Vector2f& offset, const wchar_t* text)
{
	particles_.push_front(new TextParticle(offset, text));
	SoundSystem::GetInstance().PlaySound("bonus");
}


void ParticleSystem::AddShield(int count, const sf::Sprite* handle)
{
	float angle = 2 * PI / count;
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new LinkedParticle(handle, angle * (i + 1)));
	}
}


void ParticleSystem::AddFollow(int count, const sf::Sprite* handle)
{
	for (int i = 0; i < count; ++i)
	{
		puts("lulz!");
		particles_.push_front(new Follow(handle));
	}
}


void ParticleSystem::RemoveShield(const sf::Sprite* handle)
{
	ParticleList::iterator it;
	for (it = particles_.begin(); it != particles_.end();)
	{
		LinkedParticle* p = dynamic_cast<LinkedParticle*>(*it);
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

/*void ParticleSystem::KillAllLinkedBy(const sf::Sprite* handle) */

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


void ParticleSystem::Render(sf::RenderTarget& target) const
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

// -----------

// Fiery
#define FIERY_VELOCITY          150
#define FIERY_MIN_LIFETIME      0.6f
#define FIERY_MAX_LIFETIME      3.6f

ParticleSystem::Fiery::Fiery(const sf::Vector2f& offset, const sf::Image& img)
{
	sprite_.SetImage(img);
	sprite_.SetPosition(offset);
	angle_ = sf::Randomizer::Random(-PI, PI);
	sprite_.SetRotation(math::rad_to_deg(angle_));
	lifetime_ = sf::Randomizer::Random(FIERY_MIN_LIFETIME, FIERY_MAX_LIFETIME);
	timer_ = 0.f;
}


bool ParticleSystem::Fiery::OnUpdate(float frametime)
{
	timer_ += frametime;
	int speed = static_cast<int> ((lifetime_ - timer_) * FIERY_VELOCITY * frametime);
	sprite_.Move(speed * math::cos(angle_), -speed * math::sin(angle_));
	// transparence
	sprite_.SetColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 - 255 * timer_ / lifetime_)));
	return timer_ >= lifetime_;
}


// Star
#define STAR_MIN_SPEED       30.0f
#define STAR_MAX_SPEED       1000.0f

ParticleSystem::Star::Star()
{
	sprite_.SetImage(GET_IMG("star"));
	float x = sf::Randomizer::Random(0, WIN_WIDTH);
	float y = sf::Randomizer::Random(ControlPanel::HEIGHT, WIN_HEIGHT);
	sprite_.SetPosition(x, y);
	float scale = sf::Randomizer::Random(0.5f, 2.0f);
	sprite_.SetScale(scale, scale);
	speed_ = static_cast<int> (sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED));
}


bool ParticleSystem::Star::OnUpdate(float frametime)
{
	if (sprite_.GetPosition().x < 0)
	{
		sprite_.SetX(WIN_WIDTH);
		sprite_.SetY(sf::Randomizer::Random(ControlPanel::HEIGHT, WIN_HEIGHT));
		sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED);
		float scale = sf::Randomizer::Random(0.5f, 1.5f);
		sprite_.SetScale(scale, scale);
	}
	sprite_.Move(-speed_ * frametime, 0);
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


// LinkedParticle
// distance vaisseau <-> particule (plus elle est éloignée, plus elle va vite)
#define SHIELD_RADIUS 42

ParticleSystem::LinkedParticle::LinkedParticle(const sf::Sprite* handle,
	float angle)
{
	sprite_.SetImage(GET_IMG("shield"));
	sprite_.SetCenter(sprite_.GetSize().x / 2, sprite_.GetSize().y / 2);
	handle_ = handle;

	angle_ = angle;
	sprite_.SetRotation(math::rad_to_deg(angle + 0.5 * PI));
}


bool ParticleSystem::LinkedParticle::OnUpdate(float frametime)
{
	sf::Vector2f offset = handle_->GetPosition();
	angle_ += (2 * PI * frametime); // rotation de 2 * PI par seconde
	offset.x += handle_->GetSize().x / 2;
	offset.y += handle_->GetSize().y / 2;
	offset.x = offset.x + SHIELD_RADIUS * math::cos(angle_);
	offset.y = offset.y - SHIELD_RADIUS * math::sin(angle_);
	sprite_.SetPosition(offset);
	sprite_.SetRotation(math::rad_to_deg(angle_ + (0.5 * PI)));
	return false;
}

#define MAX_LIFETIME_FOLLOW 1.5f

ParticleSystem::Follow::Follow(const sf::Sprite* handle)
{
	handle_ = handle;
	sf::Vector2f pos = handle->GetPosition();

	sprite_.SetPosition(pos.x, pos.y);
	sprite_.SetImage(GET_IMG("star"));
	speed_ = FIERY_VELOCITY;
	angle_ = sf::Randomizer::Random(0, 360);
	timer_ = sf::Randomizer::Random(0.0f, MAX_LIFETIME_FOLLOW);
}


bool ParticleSystem::Follow::OnUpdate(float frametime)
{
	timer_ += frametime;
	if (timer_ >= MAX_LIFETIME_FOLLOW)
	{
		timer_ = 0;
		sprite_.SetPosition(handle_->GetPosition().x, handle_->GetPosition().y + handle_->GetSize().y / 2);
		angle_ = sf::Randomizer::Random(0, 360);
	}


	sf::Vector2f pos = sprite_.GetPosition();
	int speed = static_cast<int> ((MAX_LIFETIME_FOLLOW - timer_) * 500 * frametime);

	math::translate(pos, angle_, speed * frametime);
	sprite_.SetPosition(pos);

	sprite_.SetColor(sf::Color(255, 255, 255, (sf::Uint8) (255 - 255 * timer_ / MAX_LIFETIME_FOLLOW)));
	/*
	sf::Vector2f offset = handle_->GetPosition();
	sf::Vector2f myp = sprite_.GetPosition();
	offset.x = offset.x + (handle_->GetSize().x / 2) + sf::Randomizer::Random(-100, 100);
	offset.y = offset.y + (handle_->GetSize().y / 2) + sf::Randomizer::Random(-100, 100);
	angle_ = math::angle(offset, myp);
	float d = math::distance(myp, offset) * 2;
	float dist =  d * frametime;
	math::translate(myp, angle_, dist);
	sprite_.SetPosition(myp);*/
	return false;
}

