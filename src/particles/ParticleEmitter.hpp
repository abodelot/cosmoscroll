#ifndef PARTICLE_EMITTER_HPP
#define PARTICLE_EMITTER_HPP

#include "ParticleSystem.hpp"

/**
 * Emitters launch particles and control their properties over their lifetime
 */
class ParticleEmitter {
public:
    ParticleEmitter(ParticleSystem& particleSystem);

    virtual ~ParticleEmitter();

    /**
     * Duration of a particle
     * @param duration: time to live in seconds. set to 0 for persistent particle.
     */
    void setLifetime(float duration);

    /**
     * If looping is enabled, particle will not be removed after its lifetime expired and be reset instead
     */
    void setLooping(bool looping);
    bool isLooping() const;

    /**
     * Set the particle color (default: white)
     * If two colors are used, particle will fade from \p start to \p end color
     */
    void setParticleColor(const sf::Color& color);
    void setParticleColor(const sf::Color& start, const sf::Color& end);

    /**
     * Set the particle direction (default: [0:360])
     * @param variation: angle can vary from angle - variation to angle + variation
     */
    void setAngle(float angle, float variation = 0.f);

    /**
     * Set the particle speed
     * @param variation: speed can vary from speed - variation to speed + variation
     */
    void setSpeed(float speed, float variation = 0.f);

    /**
     * Particle scale (default: 1)
     * Particles will scale from \p start to \p end
     */
    void setScale(float start, float end = 1.f);

    /**
     * Set the texture rect for the particles
     * If a texture is set in the particle system, whole texture is used by default.
     * Otherwise, particles default to a 1px wide square.
     */
    void setTextureRect(const sf::IntRect& rect);
    const sf::IntRect& getTextureRect() const;

    /**
     * Create particles linked to this emitter in the particle system
     */
    void createParticles(size_t count = 100);

    /**
     * Remove all particles emitted by this emitter
     */
    void clearParticles() const;

    /**
     * Reset a particle with this emitter's properties
     * @param particle: particle to be initialized
     */
    void resetParticle(Particle& particle) const;

    /**
     * Set the emitter position (where particles will be spawn at)
     */
    void setPosition(const sf::Vector2f& position);
    void setPosition(float x, float y);
    const sf::Vector2f& getPosition() const;

    /**
     * Move the emitter position
     */
    void move(const sf::Vector2f& delta);
    void move(float dx, float dy);

    /**
     * Compute color transition between start color and end color
     */
    sf::Color modulateColor(float lifespan, float elapsed) const;

    /**
     * Compute scale transition between start scale and end scale
     */
    float modulateScale(float lifespan, float elapsed) const;

    /**
     * Override this method to implement a custom behavior when particle is
     * inserted in the particle system
     */
    virtual void onParticleCreated(Particle&) const {};

    /**
     * Override this method to implement a custom behavior when particle is updated
     */
    virtual void onParticleUpdated(Particle&, float) const {};

private:
    ParticleSystem& m_particleSystem;

    sf::Vector2f m_position;
    bool         m_looping;
    float        m_lifetime;
    sf::Color    m_startColor;
    sf::Color    m_endColor;
    float        m_startScale;
    float        m_endScale;
    float        m_angle;
    float        m_angleVariation;
    float        m_speed;
    float        m_speedVariation;
    sf::IntRect  m_textureRect;
};

#endif
