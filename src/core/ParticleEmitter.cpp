#include "ParticleEmitter.hpp"
#include "utils/Math.hpp"


ParticleEmitter::ParticleEmitter():
    m_looping(false),
    m_lifetime(5),
    m_start_color(sf::Color::White),
    m_end_color(sf::Color(0, 0, 0, 0)),
    m_start_scale(1),
    m_end_scale(1),
    m_angle(0),
    m_angle_variation(math::PI * 2),
    m_speed(100),
    m_speed_variation(50)
{
    const sf::Texture* texture = ParticleSystem::getInstance().getTexture();
    if (texture != NULL)
    {
        // If particle system already provides a texture, use the whole texture
        m_texture_rect.width = texture->getSize().x;
        m_texture_rect.height = texture->getSize().y;
    }
    else
    {
        // Otherwise, particles are just 1 pixel-wide
        m_texture_rect.width = 1;
        m_texture_rect.height = 1;
    }
}


ParticleEmitter::~ParticleEmitter()
{
    clearParticles();
}


void ParticleEmitter::setLifetime(float duration)
{
    m_lifetime = duration;
}


void ParticleEmitter::setLooping(bool looping)
{
    m_looping = looping;
}


bool ParticleEmitter::isLooping() const
{
    return m_looping;
}


void ParticleEmitter::setParticleColor(const sf::Color& color)
{
    m_start_color = color;
}


void ParticleEmitter::setParticleColor(const sf::Color& start, const sf::Color& end)
{
    m_start_color = start;
    m_end_color = end;
}


void ParticleEmitter::setAngle(float angle, float variation)
{
    m_angle = angle;
    m_angle_variation = variation;
}


void ParticleEmitter::setSpeed(float speed, float variation)
{
    m_speed = speed;
    m_speed_variation = variation;
}


void ParticleEmitter::setScale(float start, float end)
{
    m_start_scale = start;
    m_end_scale = end;
}


void ParticleEmitter::setTextureRect(const sf::IntRect& rect)
{
    m_texture_rect = rect;
}


const sf::IntRect& ParticleEmitter::getTextureRect() const
{
    return m_texture_rect;
}


sf::Color ParticleEmitter::modulateColor(float lifespan, float elapsed) const
{
    if (lifespan)
        return sf::Color(
            m_start_color.r + (elapsed * (m_end_color.r - m_start_color.r) / lifespan),
            m_start_color.g + (elapsed * (m_end_color.g - m_start_color.g) / lifespan),
            m_start_color.b + (elapsed * (m_end_color.b - m_start_color.b) / lifespan),
            m_start_color.a + (elapsed * (m_end_color.a - m_start_color.a) / lifespan)
        );

    return m_start_color;
}


float ParticleEmitter::modulateScale(float lifespan, float elapsed) const
{
    if (lifespan)
        return m_start_scale + (elapsed * (m_end_scale - m_start_scale) / lifespan);

    return m_start_scale;
}


void ParticleEmitter::createParticles(size_t count)
{
    ParticleSystem& particleSystem = ParticleSystem::getInstance();

    // Insert 'count' particles in the particle system
    for (size_t i = 0; i < count; ++i)
    {
        ParticleSystem::Particle p(*this);
        resetParticle(p);
        particleSystem.addParticle(p);
    }
}


void ParticleEmitter::clearParticles() const
{
    ParticleSystem::getInstance().removeByEmitter(*this);
}


void ParticleEmitter::resetParticle(ParticleSystem::Particle& particle) const
{
    // Set position adjusted to particle width and height
    particle.position.x = m_position.x - m_texture_rect.width / 2;
    particle.position.y =  m_position.y - m_texture_rect.height / 2;

    // Set random angle
    particle.angle = math::rand(m_angle - m_angle_variation, m_angle + m_angle_variation);

    // Set random velocity vector
    float speed = math::rand(m_speed - m_speed_variation, m_speed + m_speed_variation);
    particle.velocity = sf::Vector2f(speed * std::cos(particle.angle), speed * -std::sin(particle.angle));

    // Set random lifetime
    particle.lifespan = m_lifetime == 0.f ? 0.f : math::rand(0.f, m_lifetime);
    particle.elapsed = 0.f;

    onParticleCreated(particle);
}


void ParticleEmitter::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}


void ParticleEmitter::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}


const sf::Vector2f& ParticleEmitter::getPosition() const
{
    return m_position;
}


void ParticleEmitter::move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
}
