#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "entities/Animated.hpp"
#include "entities/EntityManager.hpp"
#include "utils/sfml_helper.hpp"

/**
 * Moteur de particules pour gérer des effets graphiques (singleton)
 */
class ParticleSystem: public sf::Drawable
{
public:
	/**
	 * Récupérer l'instance unique
	 */
	static ParticleSystem& GetInstance();

	/**
	 * Add an effect in the scene
	 * @param pos: sfx origin
	 * @param count: number of particles
	 */
	void ExplosionSfx(const sf::Vector2f& pos);
	void ImpactSfx(const sf::Vector2f& pos, int count);
	void GreenImpactSfx(const sf::Vector2f& pos, int count);
	void FierySfx(const sf::Vector2f& pos, int count);
	void SnowflakeSfx(const sf::Vector2f& pos, int count);


	/**
	 * Ajouter des étoiles défilantes dans la scène
	 * @param count: nombre d'étoiles
	 */
	void AddStars(int count = 33);

	void AddCenteredStars(int count);

	/**
	 * Ajouter un message défilant
	 * @param offset: position
	 * @param text: contenu du message
	 */
	void AddMessage(const sf::Vector2f& offset, const sf::String& text, const sf::Color& color=sf::Color::White);

	void AddShield(int count, const sf::Sprite* handle);
	void RemoveShield(const sf::Sprite* handle);

	void AddSmoke(int count, const sf::Sprite* handle);
	void ClearSmoke(const sf::Sprite* handle);


	/**
	 * Mise à jour des particules (déplacement)
	 */
	void Update(float frametime);

	/**
	 * Suppression de toutes les particules
	 */
	void Clear();


private:
	ParticleSystem();
	ParticleSystem(const ParticleSystem& other);
	ParticleSystem& operator=(const ParticleSystem& other);
	~ParticleSystem();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	/**
	 * Particule abstraite
	 */
	class Particle: public sf::Drawable
	{
	public:
		/**
		 * Animation de la particule lors de l'update du ParticleSystem
		 * @return true si la particule est morte, sinon false
		 */
		virtual bool OnUpdate(float frametime) = 0;
		virtual ~Particle() {};

		/**
		 * Affichage de la particule
		 */
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
	};


	/**
	 * Impact d'une explosion
	 */
	class Fiery: public Particle
	{
	public:
		Fiery(const sf::Vector2f& offset, const sf::Texture& img);
		~Fiery() {};
		bool OnUpdate(float frametime);

		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(sprite_, states);
		}
	private:
		sf::Sprite sprite_;
		float angle_; // en radians
		float timer_;
		float lifetime_;
	};


	/**
	 * Étoile défilante
	 */
	class Star: public Particle
	{
	public:
		Star(const sf::Texture& img);
		~Star() {};
		bool OnUpdate(float frametime);

		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(sprite_, states);
		}
	protected:
		int speed_;
		xsf::Sprite sprite_;
	};

	class CenteredStar: public Star
	{
	public:
		CenteredStar(const sf::Texture& img);
		~CenteredStar() {};
		bool OnUpdate(float frametime);

	private:
		float angle_;
	};

	/**
	 * Court message défilant
	 */
	class TextParticle: public Particle
	{
	public:
		TextParticle(const sf::Vector2f& offset, const sf::String& text, const sf::Color& color);
		~TextParticle() {};
		bool OnUpdate(float frametime);

		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(text_, states);
		}
	private:
		sf::Text text_;
		float timer_;
	};


	/**
	 * Particule liée à un objet externe au ParticleSystem (non autonome)
	 */
	class ShieldParticle: public Particle
	{
	public:
		ShieldParticle(const sf::Sprite* handle, float angle);
		~ShieldParticle() {};
		bool OnUpdate(float frametime);

		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(sprite_, states);
		}

		inline const sf::Sprite* GetHandle() const
		{
			return handle_;
		}
	private:
		const sf::Sprite* handle_; // hack... le sprite cible sert de handle
		xsf::Sprite sprite_;
		float angle_; // en radians
	};

	class Smoke: public Particle
	{
	public:
		Smoke(const sf::Texture& img, const sf::Sprite* handle);
		~Smoke() { }
		bool OnUpdate(float frametime);
		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(sprite_, states);
		}
		const sf::Sprite* GetHandle() const
		{
			return handle_;
		}
		void SetHandle(const sf::Sprite* handle)
		{
			handle_ = handle;
		}
	private:
		int y_offset_;
		xsf::Sprite sprite_;
		const sf::Sprite* handle_;
		float timer_;
		sf::Vector2f vspeed_;
	};

	class Explosion: public Particle, public Animated
	{
		public:
		Explosion(const sf::Vector2f& pos) :
			Animated(EntityManager::GetInstance().GetAnimation("explosion"))
		{
			Animated::Reset(sprite_);
			sprite_.setOrigin(sprite_.getCenter());
			sprite_.setPosition(pos);
			timer_ = 0;
		}
		bool OnUpdate(float frametime)
		{
			Animated::UpdateSubRect(sprite_, frametime);
			timer_ += frametime;
			if (timer_ > Animated::GetAnimation()->GetDuration())
			{
				return true;
			}
			return false;
		}
		inline void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			target.draw(sprite_, states);
		}
		private:
		xsf::Sprite sprite_;
		float timer_;
	};
	typedef std::list<Particle*> ParticleList;

	ParticleList particles_;
};

#endif // PARTICLESYSTEM_HPP

