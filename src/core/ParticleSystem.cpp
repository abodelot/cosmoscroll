#include "ParticleSystem.hpp"
#include "ParticleEmitter.hpp"
#include "utils/Math.hpp"


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


void ParticleSystem::removeByEmitter(const ParticleEmitter& emitter)
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

ParticleSystem::Particle::Particle(const ParticleEmitter& e):
    emitter(e)
{
}
