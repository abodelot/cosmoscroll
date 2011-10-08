#ifndef PARTICLESYSTEM_HPP
#define PARTICLESYSTEM_HPP

#include <list>
#include <SFML/Graphics.hpp>

#include "entities/Animated.hpp"
#include "entities/EntityManager.hpp"
#include "utils/MediaManager.hpp"

/**
 * Moteur de particules pour gérer des effets graphiques (singleton)
 */
class ParticleSystem
{
public:
	/**
	 * Récupérer l'instance unique
	 */
	static ParticleSystem& GetInstance();

	/**
	 * Ajouter une explosion
	 * @param offset: position de l'explosion
	 */
	void AddExplosion(const sf::Vector2f& offset);
	void AddFiery(int x, int y);

	/**
	 * Ajouter un effet d'impact
	 * @param offset: position de l'impact
	 * @param count: nombre d'étincelles
	 */
	void AddImpact(const sf::Vector2f& offset, int count);
	void AddGreenImpact(const sf::Vector2f& pos, int count);

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
	void AddMessage(const sf::Vector2f& offset, const sf::Unicode::Text& text, const sf::Color& color=sf::Color::White);

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

	void Show(sf::RenderTarget& target) const;
private:
	ParticleSystem();
	ParticleSystem(const ParticleSystem& other);
	ParticleSystem& operator=(const ParticleSystem& other);
	~ParticleSystem();

	/**
	 * Particule abstraite
	 */
	class Particle
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
		virtual void Show(sf::RenderTarget& target) const = 0;
	};


	/**
	 * Impact d'une explosion
	 */
	class Fiery: public Particle
	{
	public:
		Fiery(const sf::Vector2f& offset, const sf::Image& img);
		~Fiery() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
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
		Star(const sf::Image& img);
		~Star() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}
	protected:
		int speed_;
		sf::Sprite sprite_;
	};

	class CenteredStar: public Star
	{
	public:
		CenteredStar(const sf::Image& img);
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
		TextParticle(const sf::Vector2f& offset, const sf::Unicode::Text& text, const sf::Color& color);
		~TextParticle() {};
		bool OnUpdate(float frametime);

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(text_);
		}
	private:
		sf::String text_;
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

		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}

		inline const sf::Sprite* GetHandle() const
		{
			return handle_;
		}
	private:
		const sf::Sprite* handle_; // hack... le sprite cible sert de handle
		sf::Sprite sprite_;
		float angle_; // en radians
	};

	class Smoke: public Particle
	{
	public:
		Smoke(const sf::Image& img, const sf::Sprite* handle);
		~Smoke() { }
		bool OnUpdate(float frametime);
		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
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
		float angle_;
		sf::Sprite sprite_;
		const sf::Sprite* handle_;
		int y_offset_;
		float timer_;
	};

	class Explosion: public Particle, public Animated
	{
		public:
		Explosion(const sf::Vector2f& pos) :
			Animated(EntityManager::GetInstance().GetAnimation("explosion"))
		{
			Animated::InitSprite(sprite_);
			sprite_.SetPosition(pos);
			sprite_.SetSubRect(Animated::GetAnimation().GetFrame(0));
			timer_ = 0;
		}
		bool OnUpdate(float frametime)
		{
			Animated::Update(frametime, sprite_);
			timer_ += frametime;
			if (timer_ > Animated::GetAnimation().GetDuration())
			{
				return true;
			}
			return false;
		}
		inline void Show(sf::RenderTarget& target) const
		{
			target.Draw(sprite_);
		}
		private:
		sf::Sprite sprite_;
		float timer_;
	};
	typedef std::list<Particle*> ParticleList;

	ParticleList particles_;
	const MediaManager& media_;
};

#endif // PARTICLESYSTEM_HPP

