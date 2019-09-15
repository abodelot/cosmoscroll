#include "ParticleEmitter.hpp"
#include "utils/Math.hpp"


ParticleEmitter::ParticleEmitter(ParticleSystem& particleSystem):
    m_particleSystem(particleSystem),
    m_looping(false),
    m_lifetime(5),
    m_startColor(sf::Color::White),
    m_endColor(sf::Color(0, 0, 0, 0)),
    m_startScale(1),
    m_endScale(1),
    m_angle(0),
    m_angleVariation(math::PI * 2),
    m_speed(100),
    m_speedVariation(50)
{
    const sf::Texture* texture = m_particleSystem.getTexture();
    if (texture != nullptr) {
        // If particle system already provides a texture, use the whole texture
        m_textureRect.width = texture->getSize().x;
        m_textureRect.height = texture->getSize().y;
    } else {
        // Otherwise, particles are just 1 pixel-wide
        m_textureRect.width = 1;
        m_textureRect.height = 1;
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
    m_startColor = color;
}


void ParticleEmitter::setParticleColor(const sf::Color& start, const sf::Color& end)
{
    m_startColor = start;
    m_endColor = end;
}


void ParticleEmitter::setAngle(float angle, float variation)
{
    m_angle = angle;
    m_angleVariation = variation;
}


void ParticleEmitter::setSpeed(float speed, float variation)
{
    m_speed = speed;
    m_speedVariation = variation;
}


void ParticleEmitter::setScale(float start, float end)
{
    m_startScale = start;
    m_endScale = end;
}


void ParticleEmitter::setTextureRect(const sf::IntRect& rect)
{
    m_textureRect = rect;
}


const sf::IntRect& ParticleEmitter::getTextureRect() const
{
    return m_textureRect;
}


sf::Color ParticleEmitter::modulateColor(float lifespan, float elapsed) const
{
    if (lifespan) {
        return sf::Color(
            m_startColor.r + (elapsed * (m_endColor.r - m_startColor.r) / lifespan),
            m_startColor.g + (elapsed * (m_endColor.g - m_startColor.g) / lifespan),
            m_startColor.b + (elapsed * (m_endColor.b - m_startColor.b) / lifespan),
            m_startColor.a + (elapsed * (m_endColor.a - m_startColor.a) / lifespan)
        );
    }
    return m_startColor;
}


float ParticleEmitter::modulateScale(float lifespan, float elapsed) const
{
    if (lifespan) {
        return m_startScale + (elapsed * (m_endScale - m_startScale) / lifespan);
    }
    return m_startScale;
}


void ParticleEmitter::createParticles(size_t count)
{
    // Insert 'count' particles in the particle system
    for (size_t i = 0; i < count; ++i) {
        Particle p(*this);
        resetParticle(p);
        m_particleSystem.addParticle(p);
    }
}


void ParticleEmitter::clearParticles() const
{
    m_particleSystem.removeByEmitter(*this);
}


void ParticleEmitter::resetParticle(Particle& particle) const
{
    particle.position = m_position;

    // Set random angle
    particle.angle = math::rand(m_angle - m_angleVariation, m_angle + m_angleVariation);

    // Set random velocity vector
    const float speed = math::rand(m_speed - m_speedVariation, m_speed + m_speedVariation);
    particle.velocity = {speed * std::cos(particle.angle), speed * -std::sin(particle.angle)};

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
