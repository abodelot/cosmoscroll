#include "ParticleSystem.hpp"
#include "ParticleEmitter.hpp"
#include "utils/Math.hpp"


ParticleSystem::ParticleSystem():
    m_vertices(sf::Quads, 4000),
    m_texture(nullptr),
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
    for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();) {
        if (&(it->emitter) == &emitter) {
            // Delete particle
            it = m_particles.erase(it);
        } else {
            ++it;
        }
    }
}


void ParticleSystem::update(float frametime)
{
    m_vertices.clear();

    for (ParticleList::iterator it = m_particles.begin(); it != m_particles.end();) {
        Particle& p = *it;
        p.elapsed += frametime;
        p.emitter.onParticleUpdated(p, frametime);

        // If particle is still alive
        if (p.lifespan == 0.f || p.elapsed < p.lifespan) {
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
            const float scale = p.emitter.modulateScale(p.lifespan, p.elapsed);
            const float halfW = r.width * scale / 2.f;
            const float halfH = r.height * scale / 2.f;
            vertices[0].position = {p.position.x - halfW, p.position.y - halfH};
            vertices[1].position = {p.position.x - halfW, p.position.y + halfH};
            vertices[2].position = {p.position.x + halfW, p.position.y + halfH};
            vertices[3].position = {p.position.x + halfW, p.position.y - halfH};

            // Each point is rotated around the particle center
            const float sin = -std::sin(p.angle);
            const float cos = std::cos(p.angle);

            for (int i = 0; i < 4; ++i) {
                sf::Vertex& vertex = vertices[i];

                // Update color
                vertex.color = color;

                // Translate point relative to (0, 0) origin
                vertex.position.x -= p.position.x;
                vertex.position.y -= p.position.y;

                // Rotate point
                float x = vertex.position.x * cos - vertex.position.y * sin;
                float y = vertex.position.x * sin + vertex.position.y * cos;

                // Translate point back
                vertex.position.x = x + p.position.x;
                vertex.position.y = y + p.position.y;

                m_vertices.append(vertex);
            }
            ++it;
        } else {
            if (p.emitter.isLooping()) {
                // Reset the particle and continue iteration
                p.emitter.resetParticle(p);
                ++it;
            } else {
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
