#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP

#include <map>
#include <list>
#include <SFML/Graphics.hpp>

/**
 * ParticleSystem is singleton which stores, updates and draws particles
 */
class ParticleSystem: public sf::Drawable, sf::NonCopyable
{
public:
	class Particle;

	/**
	 * Emitters launch particles and control their properties over their lifetime
	 */
	class Emitter
	{
	public:
		Emitter();
		virtual ~Emitter();

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
		 * Override this method to implement a new behavior when particle is inserted in the particle system
		 */
		virtual void onParticleCreated(Particle&) const {};

		/**
		 * Override this method to implement a new behavior when particle is updated
		 */
		virtual void onParticleUpdated(Particle&, float) const {};

	private:
		sf::Vector2f m_position;
		bool         m_looping;
		float        m_lifetime;
		sf::Color    m_start_color;
		sf::Color    m_end_color;
		float        m_start_scale;
		float        m_end_scale;
		float        m_angle;
		float        m_angle_variation;
		float        m_speed;
		float        m_speed_variation;
		sf::IntRect  m_texture_rect;
	};

	// -------------------------------------------------------------------------

	/**
	 * A single particle in the particle system
	 */
	struct Particle
	{
		Particle(const ParticleSystem::Emitter& e);

		const Emitter&  emitter;
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
	void removeByEmitter(const Emitter& emitter);

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

	typedef std::map<std::string, Emitter> EmitterMap;
	EmitterMap           m_emitters;

	sf::VertexArray      m_vertices;
	const sf::Texture*   m_texture;
	const sf::BlendMode& m_blendMode;
};

#endif // PARTICLE_SYSTEM_HPP

