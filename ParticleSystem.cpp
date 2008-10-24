#include "ParticleSystem.hpp"
#include "MediaManager.hpp"
#include "Window.hpp"
#include "Math.hpp"

#include <SFML/System.hpp>

#define PARTICLES_PER_EXPLOSION 40
#define FIERY_MIN_LIFETIME      1.0f
#define FIERY_MAX_LIFETIME      4.0f

#define STAR_MIN_SPEED          30.0f
#define STAR_MAX_SPEED          5000.0f


ParticleSystem& ParticleSystem::GetInstance()
{
    static ParticleSystem self;
    return self;
}


void ParticleSystem::AddExplosion(const sf::Vector2f& offset)
{
    // génération d'un foyer de particules
    for (unsigned int i = 0; i < PARTICLES_PER_EXPLOSION; ++i)
    {
        particles_.push_back(new Fiery(offset));
    }
    boom_sfx_.Play();
}


void ParticleSystem::AddImpact(const sf::Vector2f& offset, int n)
{
    for (int i = 0; i < n; ++i)
    {
        particles_.push_back(new Impact(offset));
    }
}


void ParticleSystem::AddStars(int count)
{
    for (int i = 0; i < count; ++i)
    {
        particles_.push_back(new Star());
    }
}


void ParticleSystem::AddShield(int count, const sf::Sprite* handle)
{
    float angle = 2 * PI / count;
    int radius = count * 7 + 15;
    for (int i = 0; i < count; ++i)
    {
        particles_.push_back(
            new LinkedParticle(handle, angle * (i + 1), radius));
    }
}


void ParticleSystem::RemoveShield(const sf::Sprite* handle)
{
    ParticleList::iterator it;
    for (it = particles_.begin(); it != particles_.end();)
    {
        LinkedParticle* p = dynamic_cast<LinkedParticle*>(*it);
        if (p != NULL and p->IsMyHandle(handle))
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


void ParticleSystem::Show(sf::RenderWindow& app)
{
    ParticleList::const_iterator it;
    for (it = particles_.begin(); it != particles_.end(); ++it)
    {
        app.Draw(**it);
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


ParticleSystem::ParticleSystem()
{
    boom_sfx_.SetBuffer(GET_SOUNDBUF("boom"));
}


ParticleSystem::~ParticleSystem()
{
    Clear();
}


ParticleSystem::Fiery::Fiery(const sf::Vector2f& offset)
{
    SetImage(GET_IMG("blue_fiery"));
    SetPosition(offset);
    float scale = sf::Randomizer::Random(0.4f, 1.2f);
    SetScale(scale, scale);
    angle_ = sf::Randomizer::Random(-PI, PI);
    speed_ = 0;
    SetRotation(RAD_TO_DEG(angle_));
    lifetime_ = sf::Randomizer::Random(FIERY_MIN_LIFETIME, FIERY_MAX_LIFETIME);
}


bool ParticleSystem::Fiery::OnUpdate(float frametime)
{
    lifetime_ -= frametime;
    // déplacement de la particule
    // la vitesse de déplacement est proportionnelle à la durée de vie
    float speed = lifetime_ * 150 * frametime; // FIXME: magique (facteur vitesse)
    sf::Vector2f offset = GetPosition();
    offset.x = offset.x + speed * std::cos(angle_);
    offset.y = offset.y - speed * std::sin(angle_);
    SetPosition(offset);
    return lifetime_ <= 0;
}


ParticleSystem::Impact::Impact(const sf::Vector2f& offset)
{
    SetImage(GET_IMG("impact"));
    SetPosition(offset);
    angle_ = sf::Randomizer::Random(-PI, PI);
    speed_ = 0;
    SetRotation(RAD_TO_DEG(angle_));
    lifetime_ = sf::Randomizer::Random(FIERY_MIN_LIFETIME, FIERY_MAX_LIFETIME);
}


ParticleSystem::Star::Star()
{
    SetImage(GET_IMG("star"));
    float x = sf::Randomizer::Random(0, WIN_WIDTH);
    float y = sf::Randomizer::Random(0, WIN_HEIGHT);
    SetPosition(sf::Vector2f(x, y));
    angle_ = PI;
    float scale = sf::Randomizer::Random(0.5f, 1.5f);
    SetScale(scale, scale);
    speed_ = sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED);
}


bool ParticleSystem::Star::OnUpdate(float frametime)
{
    if (GetPosition().x < 0)
    {
        SetX(WIN_WIDTH);
        SetY(sf::Randomizer::Random(0, WIN_HEIGHT));
        speed_ = sf::Randomizer::Random(STAR_MIN_SPEED, STAR_MAX_SPEED);
        float scale = sf::Randomizer::Random(0.5f, 1.5f);
        SetScale(scale, scale);
    }
    Move(frametime * speed_ * std::cos(angle_), 0);
    return false;
}


ParticleSystem::LinkedParticle::LinkedParticle(const sf::Sprite* handle,
    float angle, float radius)
{
    SetImage(GET_IMG("shield"));
    SetCenter(GetSize().x / 2, GetSize().y / 2);
    handle_ = handle;
    speed_ = radius; // huh, logique ! (distance vaisseau <-> particule)
    angle_ = angle;
    SetRotation(RAD_TO_DEG(angle + 0.5 * PI));
}


bool ParticleSystem::LinkedParticle::OnUpdate(float frametime)
{
    sf::Vector2f offset = handle_->GetPosition();
    angle_ += (2 * PI * frametime); // vitesse de rotation = 2 * PI par seconde
    offset.x += handle_->GetSize().x / 2;
    offset.y += handle_->GetSize().y / 2;
    offset.x = offset.x + speed_ * std::cos(angle_);
    offset.y = offset.y - speed_ * std::sin(angle_);
    SetPosition(offset);
    SetRotation(RAD_TO_DEG(angle_ + (0.5 * PI)));
    return false;
}


bool ParticleSystem::LinkedParticle::IsMyHandle(const sf::Sprite* handle)
{
    return handle_ == handle;
}


/*
ParticleSystem::Follow::Follow(const sf::Sprite* handle)
{
    handle_ = handle;
    SetPosition(handle->GetPosition());
    SetImage(GET_IMG("blue_ammo"));
    speed_ = 0.0f;
}


bool ParticleSystem::Follow::OnUpdate(float frametime)
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


