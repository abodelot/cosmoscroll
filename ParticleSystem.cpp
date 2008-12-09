#include "ParticleSystem.hpp"
#include "Window.hpp"
#include "Math.hpp"

#include <SFML/System.hpp>

#define PARTICLES_PER_EXPLOSION 40


ParticleSystem& ParticleSystem::GetInstance()
{
	static ParticleSystem self;
	return self;
}


void ParticleSystem::AddExplosion(const sf::Vector2f& offset)
{
	for (int i = 0; i < PARTICLES_PER_EXPLOSION; ++i)
	{
		particles_.push_front(new Fiery(offset, media_.GetImage("fiery")));
	}
#ifndef NO_AUDIO
	sfx_boom_.Play();
#endif
}


void ParticleSystem::AddImpact(const sf::Vector2f& offset, int count)
{
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new Fiery(offset, media_.GetImage("impact")));
	}
}


void ParticleSystem::AddStars(int count)
{
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new Star());
	}
}


void ParticleSystem::AddMessage(const sf::Vector2f& offset, const wchar_t* text)
{
	particles_.push_front(new TextParticle(offset, text));
#ifndef NO_AUDIO
	sfx_msg_.Play();
#endif
}


void ParticleSystem::AddShield(int count, const sf::Sprite* handle)
{
	float angle = 2 * PI / count;
	for (int i = 0; i < count; ++i)
	{
		particles_.push_front(new LinkedParticle(handle, angle * (i + 1)));
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


void ParticleSystem::Show(sf::RenderWindow& app) const
{
	ParticleList::const_iterator it;
	for (it = particles_.begin(); it != particles_.end(); ++it)
	{
		(**it).Show(app);
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


ParticleSystem::ParticleSystem() :
	media_(MediaManager::GetInstance())
{
#ifndef NO_AUDIO
    sfx_boom_.SetBuffer(GET_SOUNDBUF("boom"));
    sfx_msg_.SetBuffer(GET_SOUNDBUF("bonus"));
#endif
}


ParticleSystem::~ParticleSystem()
{
	Clear();
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
	sprite_.SetRotation(RAD_TO_DEG(angle_));
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
#define STAR_MAX_SPEED       3000.0f

ParticleSystem::Star::Star()
{
	sprite_.SetImage(GET_IMG("star"));
	float x = sf::Randomizer::Random(0, WIN_WIDTH);
	float y = sf::Randomizer::Random(CONTROL_PANEL_HEIGHT, WIN_HEIGHT);
	sprite_.SetPosition(x, y);
	float scale = sf::Randomizer::Random(0.5f, 1.5f);
	sprite_.SetScale(scale, scale);
	speed_ = static_cast<int> (sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED));
}


bool ParticleSystem::Star::OnUpdate(float frametime)
{
	if (sprite_.GetPosition().x < 0)
	{
		sprite_.SetX(WIN_WIDTH);
		sprite_.SetY(sf::Randomizer::Random(CONTROL_PANEL_HEIGHT, WIN_HEIGHT));
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
	sprite_.SetRotation(RAD_TO_DEG(angle + 0.5 * PI));
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
	sprite_.SetRotation(RAD_TO_DEG(angle_ + (0.5 * PI)));
	return false;
}


/*
ne pas effacer =)
Follow::Follow(const sf::Sprite* handle)
{
	handle_ = handle;
	SetPosition(handle->GetPosition());
	SetImage(GET_IMG("blue_ammo"));
	speed_ = 0.0f;
}

bool Follow::OnUpdate(float frametime)
{
	const sf::Vector2f offset = handle_->GetPosition();
	sf::Vector2f myp = GetPosition();
	
	angle_ = ANGLE(offset, myp);
	float d = Distance(myp, offset) * 2;
	float dist =  d * frametime;
	TRANSLATE(myp, angle_, dist);
	SetPosition(myp);
	return false;
}
*/

