#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <list>
#include <SFML/Graphics.hpp>

class ParticleEmitter;

/**
 * ParticleSystem is singleton which stores, updates and draws particles
 */
class ParticleSystem: public sf::Drawable, sf::NonCopyable
{
public:
    class Particle;

    // -------------------------------------------------------------------------

    /**
     * A single particle in the particle system
     */
    struct Particle
    {
        Particle(const ParticleEmitter& e);

        const ParticleEmitter&  emitter;
        sf::Vector2f    position;
        float           angle;
        sf::Vector2f    velocity;
        float           lifespan;
        float           elapsed;
    };

    // -------------------------------------------------------------------------

    /**
     * Get singleton instance
     */
    static ParticleSystem& getInstance();

    /**
     * Attach a texture to the particle system
     * All the particles are rendered using the same texture
     */
    void setTexture(const sf::Texture* texture);
    const sf::Texture* getTexture() const;

    /**
     * Set the blend mode in the particle system
     * All the particles will be rendered using the same blend mode
     */
    void setBlendMode(const sf::BlendMode& blendMode);
    const sf::BlendMode& getBlendMode() const;

    /**
     * Delete all particles emitted by a given emitter
     */
    void removeByEmitter(const ParticleEmitter& emitter);

    /**
     * Insert a new particle in the particle system
     */
    void addParticle(const Particle& particle);

    /**
     * Update all particles
     */
    void update(float frametime);

    /**
     * Delete all particles
     */
    void clear();

private:
    ParticleSystem();
    ~ParticleSystem();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    typedef std::list<Particle> ParticleList;
    ParticleList         m_particles;

    sf::VertexArray      m_vertices;
    const sf::Texture*   m_texture;
    const sf::BlendMode& m_blendMode;
};

#endif // PARTICLE_SYSTEM_HPP
