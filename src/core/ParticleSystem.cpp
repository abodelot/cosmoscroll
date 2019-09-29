#include "ParticleSystem.hpp"
#include "utils/Math.hpp"


// ParticleSystem::Emitter -----------------------------------------------------

ParticleSystem::Emitter::Emitter():
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


ParticleSystem::Emitter::~Emitter()
{
    clearParticles();
}


void ParticleSystem::Emitter::setLifetime(float duration)
{
    m_lifetime = duration;
}


void ParticleSystem::Emitter::setLooping(bool looping)
{
    m_looping = looping;
}


bool ParticleSystem::Emitter::isLooping() const
{
    return m_looping;
}


void ParticleSystem::Emitter::setParticleColor(const sf::Color& color)
{
    m_start_color = color;
}


void ParticleSystem::Emitter::setParticleColor(const sf::Color& start, const sf::Color& end)
{
    m_start_color = start;
    m_end_color = end;
}


void ParticleSystem::Emitter::setAngle(float angle, float variation)
{
    m_angle = angle;
    m_angle_variation = variation;
}


void ParticleSystem::Emitter::setSpeed(float speed, float variation)
{
    m_speed = speed;
    m_speed_variation = variation;
}


void ParticleSystem::Emitter::setScale(float start, float end)
{
    m_start_scale = start;
    m_end_scale = end;
}


void ParticleSystem::Emitter::setTextureRect(const sf::IntRect& rect)
{
    m_texture_rect = rect;
}


const sf::IntRect& ParticleSystem::Emitter::getTextureRect() const
{
    return m_texture_rect;
}


sf::Color ParticleSystem::Emitter::modulateColor(float lifespan, float elapsed) const
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


float ParticleSystem::Emitter::modulateScale(float lifespan, float elapsed) const
{
    if (lifespan)
        return m_start_scale + (elapsed * (m_end_scale - m_start_scale) / lifespan);

    return m_start_scale;
}


void ParticleSystem::Emitter::createParticles(size_t count)
{
    ParticleSystem& particleSystem = ParticleSystem::getInstance();

    // Insert 'count' particles in the particle system
    for (size_t i = 0; i < count; ++i)
    {
        Particle p(*this);
        resetParticle(p);
        particleSystem.addParticle(p);
    }
}


void ParticleSystem::Emitter::clearParticles() const
{
    ParticleSystem::getInstance().removeByEmitter(*this);
}


void ParticleSystem::Emitter::resetParticle(Particle& particle) const
{
    // Set position adjusted to particle width and height
    particle.position = sf::Vector2f(m_position.x - m_texture_rect.width / 2,
                                     m_position.y - m_texture_rect.height / 2);

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


void ParticleSystem::Emitter::setPosition(const sf::Vector2f& position)
{
    m_position = position;
}


void ParticleSystem::Emitter::setPosition(float x, float y)
{
    m_position.x = x;
    m_position.y = y;
}


const sf::Vector2f& ParticleSystem::Emitter::getPosition() const
{
    return m_position;
}


void ParticleSystem::Emitter::move(float dx, float dy)
{
    m_position.x += dx;
    m_position.y += dy;
}

// ParticleSystem --------------------------------------------------------------

ParticleSystem& ParticleSystem::getInstance()
{
    static ParticleSystem self;
    return self;
}


ParticleSystem::ParticleSystem():
    m_vertices(sf::Quads, 4000),
    m_texture(NULL),
    m_blendMode(sf::BlendAlpha)
{
}


ParticleSystem::~ParticleSystem()
{
    clear();
}


void ParticleSystem::setTexture(const sf::Texture* texture)
{
    m_texture = texture;
}


const sf::Texture* ParticleSystem::getTexture() const
{
    return m_texture;
}


void ParticleSystem::addParticle(const Particle& particle)
{
    m_particles.push_back(particle);
}


void ParticleSystem::removeByEmitter(const Emitter& emitter)
{
    for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();)
    {
        if (&(it->emitter) == &emitter)
        {
            // Delete particle
            it = m_particles.erase(it);
        }
        else
        {
            ++it;
        }
    }
}


void ParticleSystem::update(float frametime)
{
    m_vertices.clear();

    for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();)
    {
        Particle& p = *it;
        p.elapsed += frametime;
        p.emitter.onParticleUpdated(p, frametime);

        // If particle is still alive
        if (p.lifespan == 0.f || p.elapsed < p.lifespan)
        {
            // Update position
            p.position.x += p.velocity.x * frametime;
            p.position.y += p.velocity.y * frametime;

            // Update color
            sf::Color color = p.emitter.modulateColor(p.lifespan, p.elapsed);

            // Each particle is a quad
            sf::Vertex vertices[4];

            // Compute the texture coords
            const sf::IntRect& r = p.emitter.getTextureRect();
            vertices[0].texCoords = sf::Vector2f(r.left,           r.top);
            vertices[1].texCoords = sf::Vector2f(r.left,           r.top + r.height);
            vertices[2].texCoords = sf::Vector2f(r.left + r.width, r.top + r.height);
            vertices[3].texCoords = sf::Vector2f(r.left + r.width, r.top);

            // Compute the position (top, left, bottom, right)
            float scale = p.emitter.modulateScale(p.lifespan, p.elapsed);
            float right = p.position.x + r.width * scale;
            float bottom = p.position.y + r.height * scale;
            vertices[0].position  = sf::Vector2f(p.position.x, p.position.y);
            vertices[1].position  = sf::Vector2f(p.position.x, bottom);
            vertices[2].position  = sf::Vector2f(right,        bottom);
            vertices[3].position  = sf::Vector2f(right,        p.position.y);

            // Each point is rotated around the particle center
            sf::Vector2f center(p.position.x + r.width / 2, p.position.y + r.height / 2);
            float sin = -std::sin(p.angle);
            float cos =  std::cos(p.angle);

            for (int i = 0; i < 4; ++i)
            {
                sf::Vertex& vertex = vertices[i];

                // Update color
                vertex.color = color;

                // Translate point back to origin
                vertex.position.x -= center.x;
                vertex.position.y -= center.y;

                // Rotate point
                float x = vertex.position.x * cos - vertex.position.y * sin;
                float y = vertex.position.x * sin + vertex.position.y * cos;

                // Translate point back
                vertex.position.x = x + center.x;
                vertex.position.y = y + center.y;

                m_vertices.append(vertex);
            }
            ++it;
        }
        else
        {
            if (p.emitter.isLooping())
            {
                // Reset the particle and continue iteration
                p.emitter.resetParticle(p);
                ++it;
            }
            else
            {
                // Delete the current particle and continue iteration
                it = m_particles.erase(it);
            }
        }
    }
}


void ParticleSystem::clear()
{
    m_particles.clear();
    m_vertices.clear();
}


void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.texture = m_texture;
    states.blendMode = m_blendMode;
    target.draw(m_vertices, states);
}


// ParticleSystem::Particle ----------------------------------------------------

ParticleSystem::Particle::Particle(const ParticleSystem::Emitter& e):
    emitter(e)
{
}
