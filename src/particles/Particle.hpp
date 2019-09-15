#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <SFML/System/Vector2.hpp>

class ParticleEmitter;

/**
 * A single particle in the particle system
 */
struct Particle {
    Particle(const ParticleEmitter& e): emitter(e) {}

    const ParticleEmitter& emitter;
    sf::Vector2f position;
    float angle;
    sf::Vector2f velocity;
    float lifespan;
    float elapsed;
};

#endif
